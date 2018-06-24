#pragma once

#include <window/window.hpp>
#include <safe_queue.hpp>
#include <atomic>
#include <unordered_map>
#include <logger/logger.hpp>
#include <graphics_api.hpp>

#undef main

namespace neon
{
    class base_event_handler
    {
    public:
        virtual ~base_event_handler() = default;
        virtual event_category category() = 0;
        virtual void notify(event* e) = 0;
    };

    template<typename concrete_event>
    class event_handler : public base_event_handler
    {
	    void notify(event* e) override;
    public:
	    event_category category() override;
        signal<concrete_event&> on_notify;
    };

    /**
     * \brief A subscriber is composed of multiple event handlers. Each event handler deals with a concrete type of event.
     * Being a child class of subscriber allows you to subscribe to the application-wide event loop.
     */
    class event_subscriber
    {
        std::unordered_map<event_category, std::shared_ptr<base_event_handler>> notifiers_;
        static std::atomic<size_t> available_ids_;
    public:
	    void notify(event* e);
	    event_subscriber();
        template <typename T>
	    signal_id connect(std::function<void(T&)>&& slot);
        virtual ~event_subscriber() = default;
        size_t id_;
    };

    class event_dispatcher
    {
        event_queue events_;
        std::thread thread_;
        std::mutex mutex_;
        std::condition_variable c_;
        bool dispatch_events_ = true;
    public:
        event_dispatcher();
	    ~event_dispatcher();
	    void run();
        template<typename event_type, typename = std::enable_if<std::is_base_of_v<event, event_type>>>
	    void post_event(event_type&& event, event_subscriber& recipient);
    };

    class application
    {
        event_dispatcher dispatcher_;
        window window_;
        logger log_;
        bool running_ = true;
    protected:
		graphics_api graphics_;
    public:
        template<typename T, typename event_type, typename = std::enable_if<std::is_base_of_v<event, event_type>>>
	    void post_event(event_type&& event, event_subscriber& recipient);
        application(int argc, char* argv[]);
	    virtual ~application();
		virtual void draw(const game_time delta_time);
		virtual void update(const game_time delta_time);
        int run();
    };

	template <typename concrete_event>
	void event_handler<concrete_event>::notify(event* e)
	{
		if (e->category == category())
		{
			on_notify(*static_cast<concrete_event*>(e));
		}
	}

	template <typename concrete_event>
	event_category event_handler<concrete_event>::category()
	{
		return concrete_event::category();
	}

	template <typename T>
	signal_id event_subscriber::connect(std::function<void(T&)>&& slot)
	{
		auto notifier = std::make_shared<event_handler<T>>();
		notifiers_.emplace(T::category(), notifier);
		return notifier->on_notify.connect(std::move(slot));
	}

	template <typename event_type, typename>
	void event_dispatcher::post_event(event_type&& event, event_subscriber& recipient)
	{
		events_.enqueue(event_queue_item{std::make_unique<event_type>(std::forward<event_type>(event)), recipient});
	}

	template <typename T, typename event_type, typename>
	void application::post_event(event_type&& event, event_subscriber& recipient)
	{
		dispatcher_.post_event(std::move(event), recipient);
	}

    class graphics_device : public event_subscriber
    {
    public:
	    graphics_device();
    };
}