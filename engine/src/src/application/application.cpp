#include <application/application.hpp>
#include <application/game_loop.hpp>
#include <application/logger.hpp>
#include <graphics/graphics_device.hpp>

namespace moka
{
	application::application(const int argc, char* argv[])
        : log_{ filesystem::current_path() / "application.log" }
		, window_("Moka")
		, graphics_(window_)
    {}

    application::~application()
        = default;
}