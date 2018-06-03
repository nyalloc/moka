#include <application/application.hpp>
#include <iostream>
#include <game_loop/game_loop.hpp>

namespace loki
{
    std::atomic<size_t> event_subscriber::m_available_ids = {};

    event_dispatcher application::m_dispatcher = {};

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

        while(running)
        {
            m_window.update(16.0f);
            m_window.end_frame(16.0f);
        }

        return 0;
    }
}