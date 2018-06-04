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
        std::vector<event_category> m_supported_types;
        static std::atomic<size_t> m_available_ids;
    public:
        subscriber(std::initializer_list<event_category> types)
            : m_supported_types{types}, m_id(0)
        {}

        virtual ~subscriber() = default;

        size_t m_id;

        subscriber()
        {
            m_id = ++m_available_ids;
        }

        bool supports_type(const event_category event) const
        {
            return std::find(m_supported_types.begin(), m_supported_types.end(), event) != m_supported_types.end();
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
    };

    template<typename T, typename... Args, typename = std::enable_if<std::is_base_of_v<subscriber, T>>>
    safe_subscriber<T> make_safe_subscriber(Args&&... args)
    {
        return safe_subscriber<T>{ std::forward<Args>(args)... };
    }

    template<typename Ty1, typename Ty2>
    bool find_subscriber(const Ty1& vector, const Ty2& value)
    {
        return std::find(vector.begin(), vector.end(), value) != vector.end();
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
        static void try_notify(subscriber* s, event* c)
        {
            // the event has an event attatched to it!
            // does the subscriber support this event?
            if (s->supports_type(c->type))
            {
                // the subscriber supports the type!
                // notify the subscriber!
                s->notify(c);
            }
        }

        event_dispatcher()
        {
            m_thread = std::thread
            {
                [this]
                {
                    std::cout << "Starting message dispatcher thread: " << std::this_thread::get_id() << std::endl;

                    while (dispatch_events)
                    {
                        // blocking call until next event is enqueued
                        const auto& queue_item = m_events.dequeue();

                        // get event and subscriber objects
                        const auto&& q_event = std::move(queue_item.event);
                        const auto& q_subscriber = queue_item.subscriber;

                        // does the event have a subscriber attatched to it?
                        if(q_subscriber)
                        {
                            // notify the specific subscriber
                            try_notify(q_subscriber, q_event.get());
                        }
                        // this event has no subscriber attatched to it.
                        else
                        {
                            // broadcast the message!
                            for(const auto& subscriber : m_subscribers)
                            {
                                // notify the next subscriber
                                try_notify(subscriber.second.get(), q_event.get());
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

        void try_register(std::shared_ptr<subscriber> subscriber)
        {
            if (!registered(subscriber))
            {
                register_subscribers(subscriber);
            }
        }

        template<typename event_type, typename = std::enable_if<std::is_base_of_v<event, event_type>>>
        void post_event(event_type&& event, std::shared_ptr<subscriber> recipient = nullptr)
        {
            if (!registered(recipient))
            {
                register_subscribers(recipient);
            }
            m_events.enqueue(event_queue_item{ std::make_unique<event_type>(std::move(event)), recipient.get() });
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
            m_dispatcher.post_event(std::move(event), recipient.ptr);
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
            : subscriber{ event_category::test }
        {}

        void notify(event* event) override
        {
            if(event->type == event_category::test)
            {
                std::cout << "Message handled on thread: " << std::this_thread::get_id() << std::endl;
                const auto e = static_cast<test_event*>(event);
                std::cout << e->string << std::endl;
            }
        }
    };
}