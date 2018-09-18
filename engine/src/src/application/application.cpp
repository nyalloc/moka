#include <application/application.hpp>
#include <application/game_loop.hpp>
#include <application/logger.hpp>
#include <graphics/graphics_device.hpp>

namespace moka
{
	application::application(const int argc, char* argv[])
        : log_("Application", moka::log_level::info)
		, window_("Moka")
		, graphics_(window_)
		, timer_(true)
    {
		log_.info("Application started");
	}

    application::~application()
        = default;
}