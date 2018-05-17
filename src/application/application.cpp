#include <application/application.hpp>
#include <iostream>
#include <game_loop/game_loop.hpp>

namespace loki
{
    application::application(int argc, char* argv[])
        : m_window("Loki Editor")
    {
        for (auto i = 1; i < argc; ++i)
        {   
            // simply print command line arguments (for now)
            std::cout << argv[i] << std::endl;
        }
    }

    application::~application()
    = default;

    int application::run()
    {
        m_window.exit.connect([this]()
        {
            running = false;
        });

        auto update([this](game_time time)
        {
            m_window.update(time);
        });

        auto draw([this](game_time time)
        {
            m_window.end_frame(time);
        });

        game_loop game_loop(draw, update, running);

        return 0;
    }
}
