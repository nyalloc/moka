#pragma once
#include <queue>
#include <memory>
#include <mutex>

namespace moka
{
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
        safe_queue();

        ~safe_queue();

        bool empty() const;

        void enqueue(T&& t);

        T dequeue();

    private:
        std::queue<T> q_;
        mutable std::mutex m_;
        std::condition_variable c_;
    };

    template <class T>
    safe_queue<T>::safe_queue(): q_()
                                 , m_()
                                 , c_()
    {
    }

    template <class T>
    safe_queue<T>::~safe_queue()
    {
    }

    template <class T>
    bool safe_queue<T>::empty() const
    {
        return q_.empty();
    }

    template <class T>
    void safe_queue<T>::enqueue(T&& t)
    {
        std::lock_guard<std::mutex> lock(m_);
        q_.emplace(std::move(t));
        c_.notify_one();
    }

    template <class T>
    T safe_queue<T>::dequeue()
    {
        std::unique_lock<std::mutex> lock(m_);
        while (q_.empty())
        {
            c_.wait(lock);
        }
        auto val = std::move(q_.front());
        q_.pop();
        return val;
    }

    class event_subscriber;

    struct event_queue_item
    {
        event_queue_item(event_ptr&& event, moka::event_subscriber& subscriber)
            : event(std::move(event)),
            subscriber(subscriber)
        {}

        event_ptr event;
        moka::event_subscriber& subscriber;
    };

    class event_queue
    {
        safe_queue<event_queue_item> queues_;
    public:
        void enqueue(event_queue_item&& e)
        {
            queues_.enqueue(std::move(e));
        }

        event_queue_item dequeue()
        {
            return queues_.dequeue();
        }
    };
}