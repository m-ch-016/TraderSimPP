#include <gtest/gtest.h>
#include "OrderBook.h"

TEST(OrderBookTest, ValidAddOrder)
{
    OrderBook book;
    Order order(Side::BUY, 20, 100);
    book.addOrder(order);

    ASSERT_NE(book.getBestBuy(), nullptr);
    EXPECT_EQ(book.getBestBuy()->getSide(), Side::BUY);
    EXPECT_EQ(book.getBestBuy()->getPrice(), 20);
    EXPECT_EQ(book.getBestBuy()->getQuantity(), 100);
    EXPECT_GT(book.getBestBuy()->getOrderId(), 0);
}


TEST(OrderBookTest, ValidOrderMatch)
{
    OrderBook book;

    Order buyOrder(Side::BUY, 34, 123);
    Order sellOrder(Side::SELL, 23, 120);

    book.addOrder(buyOrder);
    book.addOrder(sellOrder);

    ASSERT_NE(book.getBestBuy(), nullptr);
    ASSERT_NE(book.getBestSell(), nullptr);

    EXPECT_NO_THROW(book.matchOrders());

    EXPECT_NE(book.getBestBuy(), nullptr);
    EXPECT_EQ(book.getBestBuy()->getQuantity(), 3);

    EXPECT_EQ(book.getBestSell(), nullptr);
}