#include <gtest/gtest.h>
#include "Order.h"
#include <chrono>

static std::uint64_t currentTimestamp()
{
    return static_cast<std::uint64_t>(
        std::chrono::steady_clock::now().time_since_epoch().count());
}

TEST(OrderTest, ValidCreation)
{
    EXPECT_NO_THROW(Order order(Side::BUY, 100, 1.0, currentTimestamp()));
}

TEST(OrderTest, InvalidCreation)
{
    std::uint64_t ts = currentTimestamp();
    EXPECT_THROW(Order order(Side::SELL, 0, 1.0, ts), std::invalid_argument);
    EXPECT_THROW(Order order(Side::BUY, 12, 0.0, ts), std::invalid_argument);
}

TEST(OrderTest, UniqueOrderID)
{
    auto ts = currentTimestamp();
    Order order1(Side::BUY, 1, 1.0, ts);
    Order order2(Side::BUY, 1, 1.0, ts);

    EXPECT_NE(order1.getOrderId(), order2.getOrderId());
}

TEST(OrderTest, ValidSideReturn)
{
    Order order(Side::BUY, 100, 1.0, currentTimestamp());
    EXPECT_EQ(order.getSide(), Side::BUY);
}

TEST(OrderTest, ValidPriceReturn)
{
    Order order(Side::BUY, 100, 1.0, currentTimestamp());
    EXPECT_EQ(order.getPrice(), 100);
}

TEST(OrderTest, ValidQuantityReturn)
{
    Order order(Side::BUY, 100, 1.0, currentTimestamp());
    EXPECT_DOUBLE_EQ(order.getQuantity(), 1.0);
}

TEST(OrderTest, ValidOrderIDReturn)
{
    Order order(Side::BUY, 100, 1.0, currentTimestamp());
    EXPECT_GT(order.getOrderId(), 0);
}

TEST(OrderTest, ValidTimestampReturn)
{
    auto before = currentTimestamp();
    Order order(Side::BUY, 100, 1.0, currentTimestamp());
    auto after = currentTimestamp();

    EXPECT_GE(order.getTimestamp(), before);
    EXPECT_LE(order.getTimestamp(), after);
}
