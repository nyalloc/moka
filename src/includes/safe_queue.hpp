#pragma once
#include <queue>
#include <memory>
#include <mutex>

enum class event_category
{
    window,
    keyboard,
    mouse,
    asset_loaded,
    invalid_event
};

class event
{
public:
    explicit event(const event_category type)
        : category(type)
    {}

    event_category category;
};

class window_event : public event
{
public:
    explicit window_event()
        : event(category())
    {}

    constexpr static event_category category() noexcept
    {
        return event_category::window;
    }
};

class asset_loaded_event : public event
{
public:
    std::string string;

    explicit asset_loaded_event(std::string&& string)
        : event(category()), string(string)
    {}
     
    constexpr static event_category category() noexcept
    {
        return event_category::window;
    }
};

using event_ptr = std::unique_ptr<event>;

template <class T>
class safe_queue
{
public:
    safe_queue()
        : q()
        , m()
        , c()
    {}

    ~safe_queue()
    {}

    bool empty() const
    {
        return q.empty();
    }

    void enqueue(T&& t)
    {
        std::lock_guard<std::mutex> lock(m);
        q.emplace(std::move(t));
        c.notify_one();
    }

    T dequeue()
    {
        std::unique_lock<std::mutex> lock(m);
        while (q.empty())
        {
            c.wait(lock);
        }
        auto val = std::move(q.front());
        q.pop();
        return val;
    }

private:
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable c;
};

namespace loki
{
    class event_subscriber;
}

struct event_queue_item
{
    event_queue_item(event_ptr&& event, loki::event_subscriber& subscriber)
        : event(std::move(event)),
          subscriber(subscriber)
    {}

    event_ptr event;
    loki::event_subscriber& subscriber;
};

class event_queue
{
    safe_queue<event_queue_item> m_queues;
public:
    void enqueue(event_queue_item&& e)
    {
        m_queues.enqueue(std::move(e));
    }

    event_queue_item dequeue()
    {
        return m_queues.dequeue();
    }
};