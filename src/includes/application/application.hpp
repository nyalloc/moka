#pragma once

#include <window/window.hpp>

#undef main

namespace loki
{
    class application
    {
        window m_window;
        bool running = true;
    public:
        application(int argc, char* argv[]);
        ~application();
        int run();
    };
}