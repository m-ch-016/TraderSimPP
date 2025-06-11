#include "OrderBook.h"
#include <queue>

void OrderBook::addOrder(std::unique_ptr<Order> order)
{
    Side order_side = order->getSide();
    std::uint64_t order_price = order->getPrice();

    if (order_side == Side::BUY)
    {
       buyOrders[order_price].push(std::move(order));
    
    } else {
       sellOrders[order_price].push(std::move(order));
    }
}


std::vector<Trade> OrderBook::matchOrders()
{
    std::vector<Trade> trades;

    if (buyOrders.empty() || sellOrders.empty())
        return trades;

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

        const auto& highest_buy = highest_buy_queue.front();
        const auto& lowest_sell = lowest_sell_queue.front();

        if (highest_buy->getPrice() < lowest_sell->getPrice())
            break;

        std::uint64_t minimum_quantity = std::min(highest_buy->getQuantity(), lowest_sell->getQuantity());

        trades.push_back(
            Trade {
                (highest_buy->getTimestamp() > lowest_sell->getTimestamp()) ? highest_buy->getTimestamp() : lowest_sell->getTimestamp(),
                highest_buy->getOrderId(),
                lowest_sell->getOrderId(),
                lowest_sell->getPrice(),
                minimum_quantity,
            }
        );

        highest_buy->setQuantity(highest_buy->getQuantity() - minimum_quantity);
        lowest_sell->setQuantity(lowest_sell->getQuantity() - minimum_quantity);

        if (highest_buy->getQuantity() == 0)
        {
            highest_buy_queue.pop();
            if (highest_buy_queue.empty())
            {
                buyOrders.erase(buy_iterator->first);
            }
        }

        if (lowest_sell->getQuantity() == 0)
        {
            lowest_sell_queue.pop();
            if (lowest_sell_queue.empty())
            {
                sellOrders.erase(sell_iterator->first);
            }
        }
    }
    
    return trades;
}

void OrderBook::cancelOrder(std::uint64_t orderID)
{
    auto removeOrder = [&](std::map<std::uint64_t, std::queue<std::unique_ptr<Order>>>& order_map)
    {
        for (auto i = order_map.begin(); i != order_map.end();)
        {
            std::queue<std::unique_ptr<Order>>& current_queue = i->second;
            std::queue<std::unique_ptr<Order>> temp_queue;

            bool found = false;
            
            while (!current_queue.empty())
            {
                auto& order_ptr = current_queue.front();
                current_queue.pop();
    
                if(order_ptr->getOrderId() == orderID)
                {
                    found = true;
                    continue;
                }
    
                temp_queue.push(std::move(order_ptr));
            }
    
            std::swap(current_queue, temp_queue);
            
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
    for (auto it = buyOrders.rbegin(); it != buyOrders.rend(); it++)
    {
        if (!it->second.empty())
        {
            return it->second.front().get();
        }
    }

    return nullptr;
}

const Order* OrderBook::getBestSell() const
{
    for (auto it = sellOrders.begin(); it != sellOrders.end(); it++)
    {
        if (!it->second.empty())
        {
            return it->second.front().get();
        }
    }

    return nullptr;
}