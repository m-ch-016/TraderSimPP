#include "EventEngine.h"
#include "MeanReversionStrategy.h"
#include <algorithm>
#include <memory>
#include <iostream>

EventEngine::EventEngine(OrderBook& orderBook, Strategy& strategy)
    : m_orderBook(orderBook)
    , m_strategy(strategy)
    , m_current_time(0)
{}

void EventEngine::addEvent(std::unique_ptr<Event> event)
{
    m_events.push_back(std::move(event));
}

void EventEngine::run()
{
    std::sort(m_events.begin(), m_events.end(), 
        [](const auto& a, const auto& b) {
            return a->timestamp < b->timestamp;
        });
    
    for (auto& event : m_events)
    {
        m_current_time = event->timestamp;
        processEvent(*event);
        
        if (auto* meanRev = dynamic_cast<MeanReversionStrategy*>(&m_strategy))
        {
            auto new_orders = meanRev->extractPendingOrders();
            for (auto& order : new_orders)
            {
                m_orderBook.addOrder(std::move(order));
            }
        }

    }

    m_events.clear();
}

void EventEngine::processEvent(Event& event)
{
    switch (event.type)
    {
        case EventType::OrderEvent:
        {
            OrderEvent& orderEvent = static_cast<OrderEvent&>(event);

            m_orderBook.addOrder(std::move(orderEvent.order));

            m_strategy.onOrderBookUpdate(m_orderBook, m_current_time);

            auto trades = m_orderBook.matchOrders();
            for (const auto& trade : trades)
            {
                m_strategy.onTradeExecuted(trade);
            }

            break;
        }
    }
}
