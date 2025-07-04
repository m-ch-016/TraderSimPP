#pragma once
#include "Strategy.h"
#include <unordered_set>

class MeanReversionStrategy: public Strategy {
public:
    MeanReversionStrategy(double epsilon, size_t windowSize, double orderSize);

    virtual void onOrderBookUpdate(const OrderBook& book, uint64_t timestamp) override;
    virtual void onTradeExecuted(const Trade& trade) override;
    virtual void onStart() override;
    virtual void onFinish() override;

private:
    std::deque<double> m_midPrices;
    std::unordered_set<std::uint64_t> m_orderIds;

    size_t m_windowSize;
    double m_epsilon;
    double m_orderSize;
    double m_position;
    double m_cash = 0.0;
};