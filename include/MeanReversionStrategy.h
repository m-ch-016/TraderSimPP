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
    std::vector<std::unique_ptr<Order>> extractPendingOrders();

private:
    std::deque<double> m_midPrices;
    std::unordered_set<std::uint64_t> m_orderIds;
    std::vector<std::unique_ptr<Order>> m_pendingOrders;

    size_t m_windowSize;
    double m_epsilon;
    double m_orderSize;
    double m_position;
    double m_cash = 0.0;

    double m_last_trade_price;

    double m_base_epsilon;
    size_t m_base_window_size;
    double m_base_order_size;

    double m_low_vol = 1.0;
    double m_high_vol = 2.0;

    double computeVol() const;
    void updateParameters();
};