#ifndef ORDER_H
#define ORDER_H

#include <atomic>
#include <cstdint>
#include <chrono>
#include <stdexcept>

enum class Side {
    BUY,
    SELL,
};


class Order
{
private:
    static std::atomic<std::uint64_t> m_global_counter_id;
    std::uint64_t m_order_id;
    Side m_side;
    std::uint64_t m_price;
    std::uint64_t m_quantity;
    std::chrono::steady_clock::time_point m_timestamp;

public:
    Order(Side side, std::uint64_t price, std::uint64_t quantity);
    
    std::uint64_t getOrderId() const noexcept;
    Side getSide() const noexcept;
    std::uint64_t getPrice() const noexcept;
    std::uint64_t getQuantity() const noexcept;
    std::chrono::steady_clock::time_point getTimestamp() const noexcept;
};


#endif