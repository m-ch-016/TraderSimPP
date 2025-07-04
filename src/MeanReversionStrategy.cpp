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

    uint64_t best_buy_price = best_buy_ptr->getPrice();
    uint64_t best_sell_price = best_sell_ptr->getPrice();

    double midpoint = (static_cast<double>(best_buy_price) + best_sell_price) / 2;
    m_midPrices.push_back(midpoint);

    if (m_midPrices.size() > m_windowSize)
    {
        m_midPrices.pop_front();
    }

    double sum = accumulate(m_midPrices.begin(), m_midPrices.end(), 0.0);

    double average_midPrices = sum / m_midPrices.size();

    if (midpoint < average_midPrices - m_epsilon)
    {
        std::unique_ptr<Order> new_order = std::make_unique<Order>(Side::BUY, best_buy_price, m_orderSize, timestamp);
        m_orderIds.insert(new_order->getOrderId());

    } else if (midpoint > average_midPrices + m_epsilon) 
    {
        std::unique_ptr<Order> new_order = std::make_unique<Order>(Side::SELL, best_sell_price, m_orderSize, timestamp);
        m_orderIds.insert(new_order->getOrderId());
    }

    //NEED TO ADD THESE ORDERS NOW 
}

void MeanReversionStrategy::onTradeExecuted(const Trade& trade)
{
    if (m_orderIds.contains(trade.buyerID) || m_orderIds.contains(trade.sellerID))
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
}

void MeanReversionStrategy::onStart()
{
    m_position = 0.0;
    m_cash = 0.0;
    m_midPrices.clear();
    m_orderIds.clear();

    std::cout << "Mean Reversion strategy with windowSize=" << m_windowSize << ", epsilon=" << m_epsilon << ", orderSize=" << m_orderSize << '\n';
}

void MeanReversionStrategy::onFinish()
{
    std::cout << "Strategy ended with final position=" << m_position << ", final cash= " << m_cash << '\n';
     
}
