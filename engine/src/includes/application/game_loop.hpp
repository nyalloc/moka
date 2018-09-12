#pragma once

#include <chrono>
#include <application/signal.hpp>

namespace moka
{
    using namespace std::chrono_literals;
    using namespace std::chrono;

    using game_time = float;

    class game_loop
    {
    public:
        signal<game_time> on_update = {};
        signal<game_time> on_draw = {};

        game_loop(const std::function<void(game_time)>& update, const std::function<void(game_time)>& draw, const bool& running, nanoseconds timestep = 16666666ns)
        {
            on_update.connect([update](game_time time)
            {
                update(time);
            });

            on_draw.connect([draw](game_time time)
            {
                draw(time);
            });

            using clock = high_resolution_clock;

            auto time_start = clock::now();

            while (running)
            {
                const auto delta_time = clock::now() - time_start;
                time_start = clock::now();
                on_update(static_cast<game_time>(duration_cast<duration<game_time>>(delta_time).count()));
                on_draw(static_cast<game_time>(duration_cast<duration<game_time>>(delta_time).count()));
            }
        }
    };
}
