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
