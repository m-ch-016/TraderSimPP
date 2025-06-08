#include "Order.h"
#include <stdexcept>
#include <atomic>

std::atomic<std::uint64_t> Order::m_global_counter_id{0};


Order::Order(Side side, std::uint64_t price, std::uint64_t quantity)
    : m_order_id(++m_global_counter_id)
    , m_side(side)
    , m_price(price)
    , m_quantity(quantity)
    , m_timestamp(std::chrono::steady_clock::now())
    {
        if (quantity == 0 || price == 0)
        {
            throw std::invalid_argument("Invalid arguments for quantity or price");
        }
    }

std::uint64_t Order::getOrderId() const
{
    return m_order_id;
}

Side Order::getSide() const
{
    return m_side;
}

std::uint64_t Order::getPrice() const
{
    return m_price;
}

std::uint64_t Order::getQuantity() const
{
    return m_quantity;
}

std::chrono::steady_clock::time_point Order::getTimestamp() const
{
    return m_timestamp;
}