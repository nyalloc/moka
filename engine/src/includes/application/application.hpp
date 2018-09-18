#pragma once

#include <application/window.hpp>
#include <application/logger.hpp>
#include <application/timer.hpp>
#include <graphics/graphics_device.hpp>
#include <filesystem>

namespace moka
{
    class application
    {
		std::thread thread_;
        logger log_;
        bool running_ = true;
		void poll_events();
    protected:
		timer timer_;
        window window_;
        graphics_device graphics_;
    public:
        application(int argc, char* argv[]);
        virtual ~application();
        virtual void draw(const game_time delta_time) = 0;
        virtual void update(const game_time delta_time) = 0;
        int run();
        virtual std::filesystem::path data_path() = 0;
    };
}