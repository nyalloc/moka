#pragma once

#include <window/window.hpp>

#undef main

namespace loki
{
    class base_application
    {
    public:
        virtual ~base_application() = default;
    };

    class application : public base_application
    {
        window m_window;
        bool running = true;
    public:
        application(int argc, char* argv[]);
        ~application();
        int run();
    };
}