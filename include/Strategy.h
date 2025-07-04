#ifndef STRATEGY_H
#define STRATEGY_H

#include "Trade.h"
#include "OrderBook.h"

class Strategy
{
public:
    virtual void onOrderBookUpdate(const OrderBook& book, uint64_t timestamp) = 0;
    virtual void onTradeExecuted(const Trade& trade) = 0;
    virtual void onStart() = 0;
    virtual void onFinish() = 0;
    virtual ~Strategy() = default;
};

#endif