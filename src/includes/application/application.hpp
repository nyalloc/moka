#pragma once

#include <window/window.hpp>
#include <safe_queue.hpp>
#include <atomic>
#include <unordered_map>
#include <iostream>

#undef main

namespace loki
{
    class subscriber
    {
        static std::atomic<size_t> m_available_ids;
    public:
        virtual ~subscriber() = default;
        size_t m_id;

        subscriber()
        {
            m_id = ++m_available_ids;
        }

        virtual void notify(event* event) = 0;
    };

    template<typename T>
    class safe_subscriber
    {
    public:
        std::shared_ptr<subscriber> ptr;

        template<typename... Args, typename = std::enable_if<std::is_base_of_v<subscriber, T>>>
        explicit safe_subscriber(Args&&... args)
            : ptr(std::make_shared<T>(std::forward<Args>(args)...))
        {}
    }
    ;
    template<typename T, typename... Args, typename = std::enable_if<std::is_base_of_v<subscriber, T>>>
    safe_subscriber<T> make_safe_subscriber(Args&&... args)
    {
        return safe_subscriber<T>{ std::forward<Args>(args)... };
    }

    class event_dispatcher
    {
        event_queue m_events;
        std::unordered_map<size_t, std::shared_ptr<subscriber>> m_subscribers;
        std::thread m_thread;
        std::mutex m_mutex;
        std::condition_variable c;
        bool dispatch_events = true;
    public:
        event_dispatcher()
        {
            m_thread = std::thread
            {
                [this]
                {
                    while (dispatch_events)
                    {
                        // blocking call until event is enqueued
                        const auto event = m_events.dequeue();

                        const auto event_ptr = event.get();

                        // todo: if an event is registered against a specific recipient, only notify them
                        // otherwise, loop over all subscribers and notify them all if they are interested in that event type

                        // loop over all subscribers
                        for (const auto& subscribers : m_subscribers)
                        {
                            // notify events
                            if (subscribers.second)
                            {
                                subscribers.second->notify(event_ptr);
                            }
                        }
                    }
                }
            };
        }

        ~event_dispatcher()
        {
            dispatch_events = false;
            m_thread.join();
        }

        bool registered(std::shared_ptr<subscriber> subscriber)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_subscribers.find(subscriber->m_id) != m_subscribers.end();
        }

        void register_subscribers(std::shared_ptr<subscriber> subscriber)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_subscribers[subscriber->m_id] = subscriber;
        }

        template<typename event_type, typename = std::enable_if<std::is_base_of_v<event, event_type>>>
        void post_event(std::shared_ptr<subscriber> recipient, event_type&& event)
        {
            if (!registered(recipient))
            {
                register_subscribers(recipient);
            }
            m_events.enqueue(std::make_unique<event_type>(std::move(event)));
        }
    };

    class application
    {
        event_dispatcher m_dispatcher;

        window m_window;

        bool running = true;
    public:
        template<typename T, typename event_type, typename = std::enable_if<std::is_base_of_v<event, event_type>>>
        void post_event(safe_subscriber<T>& recipient, event_type&& event)
        {
            m_dispatcher.post_event(recipient.ptr, std::move(event));
        }

        application(int argc, char* argv[]);
        ~application();
        int run();
    };

    class test_class : public subscriber
    {
    public:
        ~test_class()
        {}

        test_class()
        {}

        void notify(event* event) override
        {
            if(event->type == event_types::test)
            {
                const auto e = static_cast<test_event*>(event);
                std::cout << e->string << std::endl;
            }
        }
    };
}