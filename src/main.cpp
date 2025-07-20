#include "Order.h"
#include "OrderBook.h"
#include "EventEngine.h"
#include "MeanReversionStrategy.h"

#include <fstream>
#include <string>
#include <sstream>
#include <cstdint>
#include <iostream>
#include <vector>
#include <memory>
#include <filesystem>

int main()
{
    OrderBook book;
    MeanReversionStrategy strategy(0.4, 10, 10.0);
    EventEngine engine(book, strategy);

    std::ifstream file("data/orders.csv");

    if (!file.is_open())
    {
        std::cerr << "File opening error.\n";
        return 1;
    }

    std::string line;
    std::getline(file, line);

    uint64_t timestamp = 1;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string sideStr, priceStr, quantityStr;

        if (!std::getline(ss, sideStr, ',') ||
            !std::getline(ss, priceStr, ',') ||
            !std::getline(ss, quantityStr, ','))
        {
            std::cerr << "Malformed line: " << line << '\n';
            continue;
        }

        Side side = (sideStr == "BUY") ? Side::BUY : Side::SELL;
        uint64_t price = std::stoull(priceStr);
        double quantity = std::stod(quantityStr);

        auto order = std::make_unique<Order>(side, price, quantity, timestamp);

        auto event = std::make_unique<OrderEvent>(timestamp, std::move(order));
        engine.addEvent(std::move(event));

        ++timestamp;
    }

    strategy.onStart();

    engine.run();

    strategy.onFinish();

}