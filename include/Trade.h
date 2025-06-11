#ifndef TRADE_H
#define TRADE_H

#include <cstdint>
#include <chrono>

struct Trade {
    std::chrono::system_clock::time_point timestamp;
    std::uint64_t buyerID;
    std::uint64_t sellerID;
    std::uint64_t price;
    std::uint64_t quantity;
};
#endif