#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <queue>
#include <map>
#include <memory>
#include "Order.h"
#include "Trade.h"

class OrderBook
{
private:
    using price = std::uint64_t;

    std::map<price, std::queue<std::unique_ptr<Order>>> buyOrders;
    std::map<price, std::queue<std::unique_ptr<Order>>> sellOrders;
    
public:
    void addOrder(std::unique_ptr<Order> order);
    std::vector<Trade> matchOrders();
    void cancelOrder(std::uint64_t orderID);
    const Order* getBestBuy() const;
    const Order* getBestSell() const;

};

#endif