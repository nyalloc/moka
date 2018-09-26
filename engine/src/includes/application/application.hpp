#pragma once

#include <application/window.hpp>
#include <application/logger.hpp>
#include <application/timer.hpp>
#include <graphics/graphics_device.hpp>
#include <input/keyboard.hpp>
#include <filesystem>

namespace moka
{
	using game_time = float;

    class application
    {
		std::thread thread_;
        bool running_ = true;
		void poll_events();
    protected:
		logger log_;
		timer timer_;
        window window_;
		keyboard keyboard_;
        graphics_device graphics_;
    public:
        application(int argc, char* argv[]);
        virtual ~application();
		float elapsed() const;
        virtual void draw(const game_time delta_time) = 0;
        virtual void update(const game_time delta_time) = 0;
        int run();
        virtual std::filesystem::path data_path() = 0;
    };
}