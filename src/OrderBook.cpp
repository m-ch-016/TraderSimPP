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
