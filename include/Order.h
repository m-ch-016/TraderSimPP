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
    double m_quantity;
    std::uint64_t m_timestamp;

public:
    Order(Side side, std::uint64_t price, double quantity, std::uint64_t timestamp);
    
    std::uint64_t getOrderId() const noexcept;
    Side getSide() const noexcept;
    std::uint64_t getPrice() const noexcept;
    double getQuantity() const noexcept;
    std::uint64_t getTimestamp() const noexcept;

    void setQuantity(double quantity) noexcept;
};


#endif