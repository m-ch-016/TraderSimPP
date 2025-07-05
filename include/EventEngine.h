#pragma once
#include <vector>
#include <memory>
#include "Order.h"
#include "OrderBook.h"
#include "Strategy.h"

enum class EventType 
{
    OrderEvent
};

struct Event
{
    EventType type;
    uint64_t timestamp;
    virtual ~Event() = default;
};

struct OrderEvent : public Event 
{
    std::unique_ptr<Order> order;
    OrderEvent(uint64_t timestampIn, std::unique_ptr<Order> o) : order(std::move(o)) 
    {
        timestamp = timestampIn;
        type = EventType::OrderEvent;
    }
};

class EventEngine 
{
    
private:
    OrderBook& m_orderBook;
    Strategy& m_strategy;
    
    std::vector<std::unique_ptr<Event>> events;
    uint64_t current_time;

    void processEvent(const Event& event);

public:
    EventEngine(OrderBook& orderBook, Strategy& strategy);
    void addEvent(std::unique_ptr<Event> event);
    void run();

};