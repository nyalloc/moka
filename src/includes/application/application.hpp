#pragma once
#include <imgui.h>
#include <SDL.h>

#undef main

namespace loki
{
    class application
    {
        SDL_Window* window;
        SDL_GLContext gl_context;
        bool show_demo_window = true;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        bool running = true;
    public:
        application(int argc, char* argv[]);
        ~application();
        int run();
    };
}