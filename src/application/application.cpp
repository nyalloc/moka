#include <application/application.hpp>
#include <iostream>
#include <game_loop/game_loop.hpp>
#include <future>

namespace loki
{
    std::atomic<size_t> subscriber::m_available_ids = {};

    application::application(int argc, char* argv[])
        : m_window("Loki Editor")
    {
        std::cout << "Starting main thread: " << std::this_thread::get_id() << std::endl;

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
        m_dispatcher.run();

        m_window.exit.connect([this]()
        {
            running = false;
        });

        auto test_1 = loki::make_safe_subscriber<loki::graphics_device>();

        std::async(std::launch::async, [this, &test_1]()
        {
            std::cout << "Mesh1 imported on thread " << std::this_thread::get_id() << std::endl;
            post_event(asset_loaded{ "Mesh1" }, test_1);

            std::cout << "Mesh2 imported on thread " << std::this_thread::get_id() << std::endl;
            post_event(asset_loaded{ "Mesh2" }, test_1);

            std::cout << "Mesh3 imported on thread " << std::this_thread::get_id() << std::endl;
            post_event(asset_loaded{ "Mesh3" }, test_1);
        });

        while (running)
        {
            test_1.ptr->update();
            m_window.update(16.0f);
            m_window.end_frame(16.0f);
        }

        return 0;
    }
}
