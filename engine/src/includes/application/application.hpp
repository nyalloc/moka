#pragma once

#include <application/window.hpp>
#include <application/logger.hpp>
#include <application/timer.hpp>
#include <asset_importer/asset_importer.hpp>
#include <graphics/device/graphics_device.hpp>
#include <input/keyboard.hpp>
#include <input/mouse.hpp>
#include <filesystem>

namespace moka
{
	using game_time = float;

	class app_settings
	{
		window_settings window_settings_;
	public:

		const window_settings& get_window_settings() const
		{
			return window_settings_;
		}

		app_settings(int argc, char* argv[])
		{

		}
	};

    class application
    {
        bool running_ = true;
		void poll_events();
    protected:
		logger log_;
		timer timer_;
        window window_;
		mouse mouse_;
		keyboard keyboard_;
        graphics_device graphics_;
    public:
        application(const app_settings& settings);
		application(const application& rhs) = delete;
		application(application&& rhs) = delete;
		application& operator=(const application& rhs) = delete;
		application& operator=(application&& rhs) = delete;
		virtual ~application();
		float seconds_elapsed() const;
        virtual void draw(game_time delta_time) = 0;
        virtual void update(game_time delta_time) = 0;
        int run();
        virtual std::filesystem::path data_path() = 0;
    };
}