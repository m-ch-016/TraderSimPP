#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <queue>
#include <map>
#include "Order.h"

class OrderBook
{
private:
    using price = std::uint64_t;

    std::map<price, std::queue<Order>> buyOrders;
    std::map<price, std::queue<Order>> sellOrders;
    
public:
    void addOrder(const Order &order);
    void matchOrders();
    void cancelOrder(std::uint64_t orderID);
    const Order* getBestBuy() const;
    const Order* getBestSell() const;

};

#endif