#include "MeanReversionStrategy.h"
#include <numeric>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <memory>

MeanReversionStrategy::MeanReversionStrategy(double epsilon, size_t windowSize, double orderSize)
    : m_epsilon(epsilon),
      m_windowSize(windowSize),
      m_position(0.0),
      m_orderSize(orderSize),
      m_base_epsilon(epsilon),
      m_base_order_size(orderSize),
      m_base_window_size(windowSize),
      m_cash(0.0),
      m_low_vol(5.0),
      m_high_vol(15.0)
{
    if (windowSize <= 0 || epsilon < 0 || orderSize <= 0)
    {
        throw std::invalid_argument("Invalid arguments");
    }
}

void MeanReversionStrategy::onOrderBookUpdate(const OrderBook& book, uint64_t timestamp)
{
    const Order* best_buy_ptr = book.getBestBuy();
    const Order* best_sell_ptr = book.getBestSell();

    if (!best_buy_ptr || !best_sell_ptr) return;

    uint64_t best_buy_price = best_buy_ptr->getPrice();
    uint64_t best_sell_price = best_sell_ptr->getPrice();

    double midpoint = (static_cast<double>(best_buy_price) + best_sell_price) / 2.0;
    m_midPrices.push_back(midpoint);

    while (m_midPrices.size() > m_windowSize)
    {
        m_midPrices.pop_front();
    }

    if (m_midPrices.empty()) return;

    double sum = std::accumulate(m_midPrices.begin(), m_midPrices.end(), 0.0);
    double average_midPrices = sum / m_midPrices.size();

    constexpr size_t MAX_PENDING_ORDERS = 100;
    constexpr size_t MAX_ORDER_IDS = 1000;

    if (m_orderIds.size() > MAX_ORDER_IDS) {
        std::cout << "WARNING: Too many unexecuted orders (" << m_orderIds.size() << "), clearing old ones\n";
        m_orderIds.clear();
    }

    std::cout << "Timestamp: " << timestamp << '\n';
    std::cout << "Midpoint: " << midpoint << ", Average MidPrice: " << average_midPrices << '\n';
    std::cout << "MidPrices size: " << m_midPrices.size() << '\n';
    std::cout << "Pending orders: " << m_pendingOrders.size() << ", Order IDs tracked: " << m_orderIds.size() << '\n';

    if (m_midPrices.size() >= 2 && m_pendingOrders.size() < MAX_PENDING_ORDERS)
    {
        if (midpoint < average_midPrices - m_epsilon)
        {
            auto new_order = std::make_unique<Order>(Side::BUY, best_buy_price, m_orderSize, timestamp);
            m_orderIds.insert(new_order->getOrderId());
            m_pendingOrders.push_back(std::move(new_order));
            std::cout << "BUY order created\n";
        }
        else if (midpoint > average_midPrices + m_epsilon)
        {
            auto new_order = std::make_unique<Order>(Side::SELL, best_sell_price, m_orderSize, timestamp);
            m_orderIds.insert(new_order->getOrderId());
            m_pendingOrders.push_back(std::move(new_order));
            std::cout << "SELL order created\n";
        }
    }

    updateParameters();
}

void MeanReversionStrategy::onTradeExecuted(const Trade& trade)
{
    m_last_trade_price = trade.price;

    if (m_orderIds.erase(trade.buyerID) > 0)
    {
        m_position += trade.quantity;
        m_cash -= trade.price * trade.quantity;
    }
    else if (m_orderIds.erase(trade.sellerID) > 0)
    {
        m_position -= trade.quantity;
        m_cash += trade.price * trade.quantity;
    }
}

void MeanReversionStrategy::onStart()
{
    m_position = 0.0;
    m_cash = 0.0;
    m_midPrices.clear();
    m_orderIds.clear();
    m_pendingOrders.clear();

    std::cout << "Strategy began with starting position=" << m_position
              << ", starting cash=" << m_cash << '\n';

    std::cout << "Mean Reversion strategy with windowSize=" << m_windowSize
              << ", epsilon=" << m_epsilon
              << ", orderSize=" << m_orderSize << '\n';

    std::cout << "===================================================" << "\n";
}

void MeanReversionStrategy::onFinish()
{
    std::cout << "Strategy ended with final position=" << m_position
              << ", final cash= " << m_cash << '\n';

    std::cout << "Total P&L=" << m_cash + (m_position * m_last_trade_price) << "\n";
}

std::vector<std::unique_ptr<Order>> MeanReversionStrategy::extractPendingOrders()
{
    std::vector<std::unique_ptr<Order>> to_return;
    to_return.swap(m_pendingOrders);
    return to_return;
}

double MeanReversionStrategy::computeVol() const
{
    if (m_midPrices.size() < 3) return 0.0;

    std::vector<double> returns;
    for (size_t i = 1; i < m_midPrices.size(); ++i)
    {
        if (m_midPrices[i-1] != 0)
        {
            double return_pct = (m_midPrices[i] - m_midPrices[i-1]) / m_midPrices[i-1] * 100.0;
            returns.push_back(return_pct);
        }
    }

    if (returns.size() < 2) return 0.0;

    double mean_return = std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();

    double square_sum = 0.0;
    for (double ret : returns)
    {
        square_sum += pow(ret - mean_return, 2);
    }

    double vol = sqrt(square_sum / (returns.size()-1));

    return vol;
}

void MeanReversionStrategy::updateParameters()
{
    double vol = computeVol();

    constexpr double epsilon_min = 0.1;
    constexpr double epsilon_max = 100.0;
    constexpr double orderSize_min = 1.0;
    constexpr double orderSize_max = 1000.0;


    std::cout << "Volatility: " << vol << " (low: " << m_low_vol << ", high: " << m_high_vol << ")\n";

    if (m_midPrices.size() >= 3)
    {
        if (vol > m_high_vol)
        {
            m_epsilon = std::clamp(m_base_epsilon * 1.7, epsilon_min, epsilon_max);
            m_orderSize = std::clamp(m_base_order_size * 0.85, orderSize_min, orderSize_max);
            std::cout << "High volatility - adjusted parameters\n";

        }
        else if (vol < m_low_vol)
        {
            m_epsilon = std::clamp(m_base_epsilon * 0.4, epsilon_min, epsilon_max);
            m_orderSize = std::clamp(m_base_order_size * 2.0, orderSize_min, orderSize_max);
            std::cout << "Low volatility - adjusted parameters\n";

        }
        else
        {
            m_epsilon = m_base_epsilon;
            m_orderSize = m_base_order_size;
        }
    }
}