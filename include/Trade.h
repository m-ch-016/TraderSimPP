#ifndef TRADE_H
#define TRADE_H

#include <cstdint>
#include <chrono>

struct Trade {
    std::uint64_t timestamp;
    std::uint64_t buyerID;
    std::uint64_t sellerID;
    std::uint64_t price;
    std::uint64_t quantity;
};
#endif