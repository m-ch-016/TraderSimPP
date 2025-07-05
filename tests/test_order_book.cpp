#include <gtest/gtest.h>
#include "OrderBook.h"
#include <chrono>

static std::uint64_t currentTimestamp()
{
    return static_cast<std::uint64_t>(
        std::chrono::steady_clock::now().time_since_epoch().count());
}

TEST(OrderBookTest, ValidAddOrder)
{
    OrderBook book;
    auto order = std::make_unique<Order>(Side::BUY, 20, 100.0, currentTimestamp());
    book.addOrder(std::move(order));

    ASSERT_NE(book.getBestBuy(), nullptr);
    EXPECT_EQ(book.getBestBuy()->getSide(), Side::BUY);
    EXPECT_EQ(book.getBestBuy()->getPrice(), 20);
    EXPECT_EQ(book.getBestBuy()->getQuantity(), 100);
    EXPECT_GT(book.getBestBuy()->getOrderId(), 0);
}

TEST(OrderBookTest, ValidSingleOrderMatch)
{
    OrderBook book;

    auto buyOrder = std::make_unique<Order>(Side::BUY, 34, 123.0, currentTimestamp());
    auto sellOrder = std::make_unique<Order>(Side::SELL, 23, 120.0, currentTimestamp());

    book.addOrder(std::move(buyOrder));
    book.addOrder(std::move(sellOrder));

    ASSERT_NE(book.getBestBuy(), nullptr);
    ASSERT_NE(book.getBestSell(), nullptr);

    EXPECT_NO_THROW(book.matchOrders());

    EXPECT_NE(book.getBestBuy(), nullptr);
    EXPECT_EQ(book.getBestBuy()->getQuantity(), 3);

    EXPECT_EQ(book.getBestSell(), nullptr);
}

TEST(OrderBookTest, ValidSequentialOrderMatch)
{
    OrderBook book;

    auto buy_order_1 = std::make_unique<Order>(Side::BUY, 50, 100.0, currentTimestamp());
    auto buy_order_2 = std::make_unique<Order>(Side::BUY, 48, 90.0, currentTimestamp());

    auto sell_order_1 = std::make_unique<Order>(Side::SELL, 45, 80.0, currentTimestamp());
    auto sell_order_2 = std::make_unique<Order>(Side::SELL, 47, 70.0, currentTimestamp());
    auto sell_order_3 = std::make_unique<Order>(Side::SELL, 49, 60.0, currentTimestamp());

    auto buy_1_id = buy_order_1->getOrderId();
    auto buy_2_id = buy_order_2->getOrderId();

    book.addOrder(std::move(buy_order_1));
    book.addOrder(std::move(buy_order_2));

    auto sell_1_id = sell_order_1->getOrderId();
    auto sell_2_id = sell_order_2->getOrderId();
    auto sell_3_id = sell_order_3->getOrderId();

    book.addOrder(std::move(sell_order_1));
    book.addOrder(std::move(sell_order_2));
    book.addOrder(std::move(sell_order_3));

    EXPECT_NO_THROW(book.matchOrders());

    ASSERT_NE(book.getBestBuy(), nullptr);
    EXPECT_EQ(book.getBestBuy()->getOrderId(), buy_2_id);
    EXPECT_EQ(book.getBestBuy()->getQuantity(), 40);

    ASSERT_NE(book.getBestSell(), nullptr);
    EXPECT_EQ(book.getBestSell()->getOrderId(), sell_3_id);
    EXPECT_EQ(book.getBestSell()->getQuantity(), 60);

    EXPECT_NO_THROW(book.matchOrders());
    EXPECT_EQ(book.getBestBuy()->getOrderId(), buy_2_id);
    EXPECT_EQ(book.getBestBuy()->getQuantity(), 40);
    EXPECT_EQ(book.getBestSell()->getOrderId(), sell_3_id);
    EXPECT_EQ(book.getBestSell()->getQuantity(), 60);

    EXPECT_NO_THROW(book.matchOrders());
    EXPECT_EQ(book.getBestBuy()->getOrderId(), buy_2_id);
    EXPECT_EQ(book.getBestBuy()->getQuantity(), 40);
    EXPECT_EQ(book.getBestSell()->getOrderId(), sell_3_id);
    EXPECT_EQ(book.getBestSell()->getQuantity(), 60);
}

TEST(OrderBookTest, ValidBestBuy)
{
    OrderBook book;

    ASSERT_EQ(book.getBestBuy(), nullptr);

    auto buy_order = std::make_unique<Order>(Side::BUY, 123, 12.0, currentTimestamp());
    auto buy_id = buy_order->getOrderId();

    book.addOrder(std::move(buy_order));

    EXPECT_EQ(book.getBestBuy()->getOrderId(), buy_id);
}

TEST(OrderBookTest, ValidBestSell)
{
    OrderBook book;

    ASSERT_EQ(book.getBestSell(), nullptr);

    auto sell_order = std::make_unique<Order>(Side::SELL, 123, 123.0, currentTimestamp());
    auto sell_id = sell_order->getOrderId();

    book.addOrder(std::move(sell_order));

    EXPECT_EQ(book.getBestSell()->getOrderId(), sell_id);
}

TEST(OrderBookTest, ValidCancelOrder)
{
    OrderBook book;

    auto buy_order_1 = std::make_unique<Order>(Side::BUY, 100, 12.0, currentTimestamp());
    auto buy_order_2 = std::make_unique<Order>(Side::BUY, 90, 23.0, currentTimestamp());

    auto sell_order_1 = std::make_unique<Order>(Side::SELL, 200, 23.0, currentTimestamp());
    auto sell_order_2 = std::make_unique<Order>(Side::SELL, 201, 32.0, currentTimestamp());

    auto buy_1_id = buy_order_1->getOrderId();
    auto buy_2_id = buy_order_2->getOrderId();

    auto sell_1_id = sell_order_1->getOrderId();
    auto sell_2_id = sell_order_2->getOrderId();

    book.addOrder(std::move(buy_order_1));
    book.addOrder(std::move(buy_order_2));
    book.addOrder(std::move(sell_order_1));
    book.addOrder(std::move(sell_order_2));

    ASSERT_NE(book.getBestBuy(), nullptr);

    book.cancelOrder(book.getBestBuy()->getOrderId());
    EXPECT_EQ(book.getBestBuy()->getOrderId(), buy_2_id);

    book.cancelOrder(book.getBestSell()->getOrderId());
    EXPECT_EQ(book.getBestSell()->getOrderId(), sell_2_id);

    EXPECT_NO_THROW(book.cancelOrder(1111));

    book.cancelOrder(book.getBestBuy()->getOrderId());
    book.cancelOrder(book.getBestSell()->getOrderId());

    EXPECT_EQ(book.getBestBuy(), nullptr);
    EXPECT_EQ(book.getBestSell(), nullptr);
}
