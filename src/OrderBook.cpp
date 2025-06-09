#include "OrderBook.h"
#include <queue>

void OrderBook::addOrder(const Order &order)
{
    Side order_side = order.getSide();
    std::uint64_t order_price = order.getPrice();

    if (order_side == Side::BUY)
    {
       buyOrders[order_price].push(order);
    
    } else {
       sellOrders[order_price].push(order);
    }
}


void OrderBook::matchOrders()
{
    if (buyOrders.empty() || sellOrders.empty())
        return;

    while (true)
    {
        if (buyOrders.empty() || sellOrders.empty())
            break;

        auto buy_iterator = buyOrders.rbegin();
        auto sell_iterator = sellOrders.begin();

        if (buy_iterator->second.empty())
        {
            buyOrders.erase(buy_iterator->first);
            continue;
        }
        if (sell_iterator->second.empty())
        {
            sellOrders.erase(sell_iterator->first);
            continue;
        }

        auto& highest_buy_queue = buy_iterator->second;
        auto& lowest_sell_queue = sell_iterator->second;

        Order* highest_buy = &(highest_buy_queue.front());
        Order* lowest_sell = &(lowest_sell_queue.front());

        if (highest_buy->getPrice() < lowest_sell->getPrice())
            break;

        std::uint64_t minimum_quantity = std::min(highest_buy->getQuantity(), lowest_sell->getQuantity());

        highest_buy->setQuantity(highest_buy->getQuantity() - minimum_quantity);
        lowest_sell->setQuantity(lowest_sell->getQuantity() - minimum_quantity);

        if (highest_buy->getQuantity() == 0)
            highest_buy_queue.pop();

        if (lowest_sell->getQuantity() == 0)
            lowest_sell_queue.pop();
    }
}

void OrderBook::cancelOrder(std::uint64_t orderID)
{
    auto removeOrder = [&](std::map<std::uint64_t, std::queue<Order>>& order_map)
    {
        for (auto i = order_map.begin(); i != order_map.end();)
        {
            std::queue<Order>& current_queue = i->second;
            std::queue<Order> temp_queue;

            bool found = false;
            
            while (!current_queue.empty())
            {
                Order order = current_queue.front();
                current_queue.pop();
    
                if(order.getOrderId() == orderID)
                {
                    found = true;
                    continue;
                }
    
                temp_queue.push(order);
            }
    
            current_queue = temp_queue;
    
            if (current_queue.empty())
            {
                i = order_map.erase(i);
            } else {
                ++i;
            }

            if (found) 
            {
                return true;
            }
        }

        return false;
    };

    if (removeOrder(buyOrders))
    {
        return;
    }

    removeOrder(sellOrders);
}

const Order* OrderBook::getBestBuy() const
{
    if (!buyOrders.empty())
    {
        const auto& highest_queue = buyOrders.rbegin()->second;
        if (!highest_queue.empty())
        {
            return &highest_queue.front();
        }
    }

    return nullptr;
}

const Order* OrderBook::getBestSell() const
{
    if (!sellOrders.empty())
    {
        const auto& lowest_queue = sellOrders.begin()->second;
        if (!lowest_queue.empty())
        {
            return &lowest_queue.front();
        }
    }

    return nullptr;
}