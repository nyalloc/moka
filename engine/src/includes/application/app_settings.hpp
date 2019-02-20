#pragma once

#include "window.hpp"

namespace moka
{
    struct app_settings
    {
        window_settings window_settings;

        app_settings(int argc, char* argv[]);
    };
} // namespace moka
