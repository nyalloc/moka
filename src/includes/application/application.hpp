#pragma once

#include <window/window.hpp>
#include <safe_queue.hpp>
#include <atomic>
#include <unordered_map>
#include <iostream>

#undef main

namespace loki
{
    template<typename Derived, typename Base>
    std::unique_ptr<Derived>static_unique_ptr_cast(std::unique_ptr<Base>&& p)
    {
        return std::unique_ptr<Derived>(static_cast<Derived *>(p.release()));
    }

    class subscriber
    {
        std::vector<event_category> m_supported_types;
        static std::atomic<size_t> m_available_ids;
    public:
        subscriber(std::initializer_list<event_category> types)
            : m_supported_types{types}, m_id(0)
        {}

        virtual ~subscriber() = default;

        virtual void update() = 0;

        size_t m_id;

        subscriber()
        {
            m_id = ++m_available_ids;
        }

        bool supports_type(const event_category event) const
        {
            return std::find(m_supported_types.begin(), m_supported_types.end(), event) != m_supported_types.end();
        }

        virtual void notify(event_ptr&& event) = 0;
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
        static void try_notify(subscriber* s, event_ptr&& c)
        {
            // the event has an event attatched to it!
            // does the subscriber support this event?
            if (s->supports_type(c->type))
            {
                // the subscriber supports the type!
                // notify the subscriber!
                s->notify(std::move(c));
            }
        }

        event_dispatcher()
        {}

        ~event_dispatcher()
        {
            dispatch_events = false;
            m_thread.join();
        }

        void run()
        {
            m_thread = std::thread
            {
            [this]
            {
                std::cout << "Starting message dispatcher thread: " << std::this_thread::get_id() << std::endl;

                while (dispatch_events)
                {
                    // blocking call until next event is enqueued
                    auto&& queue_item = m_events.dequeue();

                    // get event and subscriber objects
                    auto&& q_event = std::move(queue_item.event);
                    const auto& q_subscriber = queue_item.subscriber;

                    // does the event have a subscriber attatched to it?
                    if (q_subscriber)
                    {
                        // notify the specific subscriber
                        try_notify(q_subscriber, std::move(q_event));
                    }
                    // this event has no subscriber attatched to it.
                    else
                    {
                        // broadcast the message!
                        for (const auto& subscriber : m_subscribers)
                        {
                            // notify the next subscriber
                            try_notify(subscriber.second.get(), std::move(q_event));
                        }
                    }
                }
            }
            };
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
        void post_event(event_type&& event, safe_subscriber<T>& recipient)
        {
            m_dispatcher.post_event(std::move(event), recipient.ptr);
        }

        application(int argc, char* argv[]);
        ~application();
        int run();
    };

    class graphics_device : public subscriber
    {
        std::queue<std::unique_ptr<asset_loaded>> m_assets_pending;
    public:
        ~graphics_device()
        {}

        graphics_device()
            : subscriber{ event_category::asset_loaded }
        {}

        static void submit_to_gpu(std::unique_ptr<asset_loaded>&& event)
        {
            std::cout << event->string << " Submitted to GPU on thread " << std::this_thread::get_id() << std::endl;
        }

        void update() override
        {
            while(!m_assets_pending.empty())
            {
                submit_to_gpu(std::move(m_assets_pending.front()));
                m_assets_pending.pop();
            }
        }

        void notify(event_ptr&& event) override
        {
            if(event->type == event_category::asset_loaded)
            {
                m_assets_pending.emplace(static_unique_ptr_cast<asset_loaded>(std::move(event)));
            }
        }
    };
}