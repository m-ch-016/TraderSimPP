#include "Order.h"
#include <stdexcept>
#include <atomic>

std::atomic<std::uint64_t> Order::m_global_counter_id{0};


Order::Order(Side side, std::uint64_t price, double quantity, std::uint64_t timestamp)
    : m_order_id(++m_global_counter_id)
    , m_side(side)
    , m_price(price)
    , m_quantity(quantity)
    , m_timestamp(timestamp)
    {
        if (quantity == 0 || price == 0)
        {
            throw std::invalid_argument("Invalid arguments for quantity or price");
        }
    }

std::uint64_t Order::getOrderId() const noexcept
{
    return m_order_id;
}

Side Order::getSide() const noexcept
{
    return m_side;
}

std::uint64_t Order::getPrice() const noexcept
{
    return m_price;
}

double Order::getQuantity() const noexcept
{
    return m_quantity;
}

std::uint64_t Order::getTimestamp() const noexcept
{
    return m_timestamp;
}

void Order::setQuantity(double quantity) noexcept
{
    m_quantity = quantity;
}