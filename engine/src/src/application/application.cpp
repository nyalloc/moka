#include <application/application.hpp>
#include <application/game_loop.hpp>
#include <application/logger.hpp>
#include <graphics/graphics_device.hpp>

namespace moka
{
	app::app(const app_settings& app_settings)
        : log_("app", moka::log_level::info)
		, window_(app_settings.get_window_settings())
		, graphics_(window_)
		, timer_(true)
    {
		log_.info("Application started");
	}

	float app::elapsed() const 
	{
		return timer_.elapsed() / 1000.0f;
	}

    app::~app()
        = default;
}