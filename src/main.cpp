#include "Order.h"
#include "OrderBook.h"
#include <fstream>
#include <string>
#include <sstream>
#include <cstdint>
#include <iostream>
#include <vector>
#include <memory>

int main()
{
    OrderBook book;
    std::ifstream file("data/orders.csv");

    if (!file.is_open())
    {
        std::cerr << "File opening error.\n";
        return 1;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::vector<std::string> fields;
        std::string field;
        std::stringstream lineStream(line);

        while (std::getline(lineStream, field, ','))
        {
            fields.push_back(field);
        }
        
        Side order_side;
        if (fields[0] == "BUY")
        {
            order_side = Side::BUY;   
        } else {
            order_side = Side::SELL;
        }
        
        std::unique_ptr<Order> new_order = std::make_unique<Order>(order_side, std::stoull(fields[1]), std::stoull(fields[2]));
        book.addOrder(std::move(new_order));
        
        std::vector<Trade> results = book.matchOrders();
        for (const auto& trade : results)
        {
            std::time_t time = std::chrono::system_clock::to_time_t(trade.timestamp);

            std::string time_str = std::ctime(&time);
            time_str.pop_back();

            std::cout << "Trade: Buy " << trade.quantity << " @ " << time_str << " between " << trade.buyerID << " and " << trade.sellerID << " at" "\n";
        }
    }



}