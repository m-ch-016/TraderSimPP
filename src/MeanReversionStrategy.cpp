#include "MeanReversionStrategy.h"
#include <numeric>
#include <iostream>

MeanReversionStrategy::MeanReversionStrategy(double epsilon, size_t windowSize, double orderSize)
    : m_epsilon(epsilon),
      m_windowSize(windowSize),
      m_position(0.0),
      m_orderSize(orderSize)
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

    if (!best_buy_ptr || !best_sell_ptr) return;  // Safety: avoid null dereference

    uint64_t best_buy_price = best_buy_ptr->getPrice();
    uint64_t best_sell_price = best_sell_ptr->getPrice();

    double midpoint = (static_cast<double>(best_buy_price) + best_sell_price) / 2.0;
    m_midPrices.push_back(midpoint);

    if (m_midPrices.size() > m_windowSize)
    {
        m_midPrices.pop_front();
    }

    double sum = std::accumulate(m_midPrices.begin(), m_midPrices.end(), 0.0);
    double average_midPrices = sum / m_midPrices.size();
    
    std::cout << "\n";
    std::cout << "Timestamp: " << timestamp << '\n';
    std::cout << "Best Buy Price: " << best_buy_price << ", Best Sell Price: " << best_sell_price << '\n';
    std::cout << "Midpoint: " << midpoint << '\n';
    std::cout << "Price window: ";
    for (auto p : m_midPrices) std::cout << p << " ";
    std::cout << '\n';
    std::cout << "Average MidPrice: " << average_midPrices << '\n';

    if (midpoint < average_midPrices - m_epsilon)
    {
        auto new_order = std::make_unique<Order>(Side::BUY, best_buy_price, m_orderSize, timestamp);
        m_orderIds.insert(new_order->getOrderId());
        m_pendingOrders.push_back(std::move(new_order));
    }
    else if (midpoint > average_midPrices + m_epsilon)
    {
        auto new_order = std::make_unique<Order>(Side::SELL, best_sell_price, m_orderSize, timestamp);
        m_orderIds.insert(new_order->getOrderId());
        m_pendingOrders.push_back(std::move(new_order));
    }
}

void MeanReversionStrategy::onTradeExecuted(const Trade& trade)
{
    if (m_orderIds.contains(trade.buyerID))
    {
        m_position += trade.quantity;
        m_cash -= trade.price * trade.quantity;
    }
    else if (m_orderIds.contains(trade.sellerID))
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
}

std::vector<std::unique_ptr<Order>> MeanReversionStrategy::extractPendingOrders()
{
    std::vector<std::unique_ptr<Order>> to_return;
    to_return.swap(m_pendingOrders);
    return to_return;
}
