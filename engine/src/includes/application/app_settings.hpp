#pragma once

#include "window.hpp"

namespace moka
{
	class app_settings
	{
		window_settings window_settings_;
	public:

		const window_settings& get_window_settings() const;

		app_settings(int argc, char* argv[]);
	};
}
