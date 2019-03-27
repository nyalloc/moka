/*
===========================================================================
Moka Source Code
Copyright 2019 Stuart Adams. All rights reserved.
https://github.com/stuartdadams/moka
stuartdadams | linkedin.com/in/stuartdadams

This file is part of the Moka Real-Time Physically-Based Rendering Project.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

===========================================================================
*/
#include <application/application.hpp>
#include <application/logger.hpp>
#include <graphics/device/graphics_device.hpp>

namespace moka
{
    application::application(const app_settings& app_settings)
        : log_("app", moka::log_level::info),
          timer_(true),
          window_(app_settings.window),
          graphics_(window_)
    {
        log_.info("Application started");
    }

    float application::seconds_elapsed() const
    {
        return timer_.elapsed() / 1000.0f;
    }

    int application::run()
    {
        try
        {
            window_.exit.connect([this]() {
                log_.info("Exiting application");
                running_ = false;
            });

            auto t = 0.0f;
            const auto fixed_update_time = 1.0f / 60.0f;

            auto current_time = seconds_elapsed();
            auto delta_time = 0.0f;

            poll_events();
            update(fixed_update_time);

            while (running_)
            {
                const auto new_time = seconds_elapsed();
                const auto frame_time = new_time - current_time;
                current_time = new_time;

                delta_time += frame_time;

                while (delta_time >= fixed_update_time)
                {
                    mouse_.state_.motion_ = {0, 0};
                    mouse_.state_.scroll_ = {0, 0};

                    poll_events();
                    update(fixed_update_time);

                    delta_time -= fixed_update_time;
                    t += fixed_update_time;
                }

                draw(delta_time);
            }
        }
        catch (const std::exception& e)
        {
            log_.error(e.what());
        }

        return 0;
    }

    application::~application() = default;
} // namespace moka
