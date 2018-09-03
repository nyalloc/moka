#include <application/application.hpp>
#include <iostream>
#include <game_loop/game_loop.hpp>
#include <future>
#include <logger/logger.hpp>
#include <graphics/graphics_device.hpp>

namespace moka
{
    application::application(const int argc, char* argv[])
        : log_{ filesystem::current_path() / "application.log" }, window_("moka Editor")
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
        graphics_.check_errors();
		window_.swap_buffer();
    }

    void application::update(const game_time delta_time)
    {
    }
}