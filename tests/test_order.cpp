#include <gtest/gtest.h>
#include "Order.h"

TEST(OrderTest, ValidCreation)
{
    EXPECT_NO_THROW(Order order(Side::BUY, 100, 1));
}

TEST(OrderTest, InvalidCreation)
{
    EXPECT_THROW(Order order(Side::SELL, 0, 1), std::invalid_argument);
    EXPECT_THROW(Order order(Side::BUY, 12, 0), std::invalid_argument);
}

TEST(OrderTest, UniqueOrderID)
{
    Order order1(Side::BUY, 1, 1);
    Order order2(Side::BUY, 1, 1);

    EXPECT_NE(order1.getOrderId(), order2.getOrderId());
}

TEST(OrderTest, ValidSideReturn)
{
    Order order(Side::BUY, 100, 1);
    EXPECT_EQ(order.getSide(), Side::BUY);
}

TEST(OrderTest, ValidPriceReturn)
{
    Order order(Side::BUY, 100, 1);
    EXPECT_EQ(order.getPrice(), 100);
}

TEST(OrderTest, ValidQuantityReturn)
{
    Order order(Side::BUY, 100, 1);
    EXPECT_EQ(order.getQuantity(), 1);
}

TEST(OrderTest, ValidOrderIDReturn)
{
    Order order(Side::BUY, 100, 1);
    EXPECT_GT(order.getOrderId(), 0);
}

TEST(OrderTest, ValidTimeStamp)
{
    auto before = std::chrono::steady_clock::now();
    Order order(Side::BUY, 100, 1);
    auto after = std::chrono::steady_clock::now();
    
    EXPECT_GT(order.getTimestamp(), before);
    EXPECT_LT(order.getTimestamp(), after);

}

