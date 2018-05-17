#include <window/window.hpp>
#include <SDL.h>
#include <GL/glew.h>
#include <iostream>

namespace loki
{
    class window::window_impl
    {
        SDL_Window* window;
        SDL_GLContext gl_context;
    public:
        window_impl(const std::string& title, window_flags flags)
        {
            // Setup SDL
            if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
            {
                SDL_DisplayMode current;
                // Setup window
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
                SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
                SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
                SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
                SDL_GetCurrentDisplayMode(0, &current);
                window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
                gl_context = SDL_GL_CreateContext(window);
                SDL_GL_SetSwapInterval(1); // Enable vsync

                glewExperimental = GL_TRUE;
                if (glewInit() != GLEW_OK)
                {
                    std::cout << "Fatal Error: GLEW not initialized correctly." << std::endl;
                    std::abort();
                }
            }
            else
            {
                printf("Error: %s\n", SDL_GetError());
            }
        }

        ~window_impl()
        {
            SDL_GL_DeleteContext(gl_context);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }
    };
    
    window::window(const std::string& title, window_flags flags)
        : impl_(std::make_unique<window_impl>(title, flags))
    {
    }
}
