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


TEST(OrderBookTest, ValidSingleOrderMatch)
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


TEST(OrderBookTest, ValidSequentialOrderMatch)
{
    OrderBook book;

    Order buy_order_1(Side::BUY, 50, 100);  
    Order buy_order_2(Side::BUY, 48, 90);   

    
    Order sell_order_1(Side::SELL, 45, 80); 
    Order sell_order_2(Side::SELL, 47, 70); 
    Order sell_order_3(Side::SELL, 49, 60); 

    
    book.addOrder(buy_order_1);  
    book.addOrder(buy_order_2);

    book.addOrder(sell_order_1); 
    book.addOrder(sell_order_2); 
    book.addOrder(sell_order_3); 

    
    EXPECT_NO_THROW(book.matchOrders());

    ASSERT_NE(book.getBestBuy(), nullptr);
    ASSERT_EQ(book.getBestBuy()->getOrderId(), buy_order_1.getOrderId());
    EXPECT_EQ(book.getBestBuy()->getQuantity(), 20);  

    ASSERT_NE(book.getBestSell(), nullptr);
    EXPECT_EQ(book.getBestSell()->getOrderId(), sell_order_2.getOrderId());
    EXPECT_EQ(book.getBestSell()->getQuantity(), 70); 

    
    EXPECT_NO_THROW(book.matchOrders());

    ASSERT_NE(book.getBestBuy(), nullptr);
    EXPECT_EQ(book.getBestBuy()->getOrderId(), buy_order_2.getOrderId()); 
    EXPECT_EQ(book.getBestBuy()->getQuantity(), 90);

    ASSERT_NE(book.getBestSell(), nullptr);
    EXPECT_EQ(book.getBestSell()->getOrderId(), sell_order_3.getOrderId()); 
    EXPECT_EQ(book.getBestSell()->getQuantity(), 60);  

    
    EXPECT_NO_THROW(book.matchOrders());

    ASSERT_NE(book.getBestBuy(), nullptr);
    EXPECT_EQ(book.getBestBuy()->getOrderId(), buy_order_2.getOrderId());
    EXPECT_EQ(book.getBestBuy()->getQuantity(), 30);  

    
    EXPECT_EQ(book.getBestSell(), nullptr);
}

TEST(OrderBookTest, ValidBestBuy)
{
    OrderBook book;

    ASSERT_EQ(book.getBestBuy(), nullptr);

    Order buy_order(Side::BUY, 123, 12);
    book.addOrder(buy_order);

    EXPECT_EQ(book.getBestBuy()->getOrderId(), buy_order.getOrderId());
}