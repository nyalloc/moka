#pragma once
#include "signal.hpp"
#include <SDL.h>

#undef main

namespace loki
{
    class window
    {
        SDL_Window* sdl_window;
    public:
        window()
        {
            sdl_window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
        }
    };

    class application
    {
        window window;
        signal<> on_quit;
        bool running = true;

    public:
        application(int argc, char* argv[])
        {
            
        }

        int run()
        {
            while(running)
            {
                
            }
            return 0;
        }

        void quit()
        {
            on_quit();
            running = false;
        }
    };
}
