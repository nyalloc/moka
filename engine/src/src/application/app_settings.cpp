
#include <application/app_settings.hpp>

namespace moka
{
	const window_settings& app_settings::get_window_settings() const
	{
		return window_settings_;
	}

	app_settings::app_settings(int argc, char* argv[])
	{
	}
}
