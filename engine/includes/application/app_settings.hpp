#pragma once

#include <application/window.hpp>

namespace moka
{
    /**
     * \brief A simple struct that will contain the configuration for the application's rendering + windowing settings.
     */
    struct app_settings
    {
        /**
         * \brief The window settings of the application.
         */
        window_settings window;

        app_settings();
    };
} // namespace moka
