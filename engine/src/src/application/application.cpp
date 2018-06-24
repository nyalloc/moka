#include <application/application.hpp>
#include <iostream>
#include <game_loop/game_loop.hpp>
#include <future>
#include <logger/logger.hpp>
#include "graphics_api.hpp"

namespace neon
{
    std::atomic<size_t> event_subscriber::available_ids_ = {};

	void event_subscriber::notify(event* e)
	{
		auto notifier = notifiers_.at(e->category);
		notifier->notify(e);
	}

	event_subscriber::event_subscriber()
	{
		id_ = ++available_ids_;
	}

	event_dispatcher::event_dispatcher()
	{
	}

	event_dispatcher::~event_dispatcher()
	{
		dispatch_events_ = false;
		thread_.join();
	}

	void event_dispatcher::run()
	{
		thread_ = std::thread
		{
			[this]
			{
				logger event_logger{filesystem::current_path() / "event.log"};

				std::stringstream msg;
				msg << "Starting message dispatcher thread: " << std::this_thread::get_id();
				event_logger.log(level::info, msg.str());

				while (dispatch_events_)
				{
					// blocking call until next event is enqueued
					auto&& queue_item = events_.dequeue();

					event_logger.log(level::info, "Message recived in system event dispatcher");

					// get event
					auto&& event = std::move(queue_item.event);

					// get subscriber
					auto& event_subscriber = queue_item.subscriber;

					// notify the subscriber
					event_subscriber.notify(event.get());
				}
			}
		};
	}

	application::application(int argc, char* argv[])
        : window_("Neon Editor"), log_{ filesystem::current_path() / "application.log" }
    {
        std::stringstream msg;
        msg << "Starting main thread: " << std::this_thread::get_id();
        log_.log(level::info, msg.str());

        for (auto i = 1; i < argc; ++i)
        {
            // simply print command line arguments (for now)
            std::cout << argv[i] << std::endl;
        }
    }

    application::~application()
        = default;

	void application::draw(const game_time delta_time)
	{
		window_.end_frame(delta_time);
	}

	void application::update(const game_time delta_time)
	{
		window_.update(delta_time);
	}

	int application::run()
    {
        dispatcher_.run();

        window_.exit.connect([this]()
        {
            log_.log(level::info, "Exiting application");
            running_ = false;
        });

        while (running_)
        {
			update(16.0f);
			draw(16.0f);
        }

        return 0;
    }

	graphics_device::graphics_device()
	{
		connect<asset_loaded_event>([](asset_loaded_event& event)
		{
		});

		connect<window_event>([](window_event& event)
		{
		});
	}
}
