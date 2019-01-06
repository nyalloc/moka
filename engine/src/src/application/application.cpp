#include <application/application.hpp>
#include <application/game_loop.hpp>
#include <application/logger.hpp>
#include <graphics/device/graphics_device.hpp>

namespace moka
{
	application::application(const app_settings& app_settings)
        : log_("app", moka::log_level::info)
		, timer_(true)
		, window_(app_settings.get_window_settings())
		, graphics_(window_)
    {
		log_.info("Application started");
	}

	float application::seconds_elapsed() const 
	{
		return timer_.elapsed() / 1000.0f;
	}

    application::~application() = default;
}