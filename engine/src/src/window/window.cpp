#include <window/window.hpp>
#include <GL/glew.h>
#include <SDL.h>
#include <logger/logger.hpp>

namespace neon
{
    class window::impl
    {
        SDL_Window* window;
        SDL_GLContext gl_context;
        bool running;
        logger window_log{ filesystem::current_path() / "window.log" };
    public:
        signal<> exit;

        void end_frame(game_time delta_time) const
        {
            SDL_GL_SwapWindow(window);
        }

        impl(const std::string& title)
            : running(true)
        {
            window_log.log(level::info, "Initializing main Window, creating OpenGL context.");

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
                    window_log.log(level::fatal, "Failed to initialize GLEW");
                    std::abort();
                }
            }
            else
            {
                window_log.log(level::error, SDL_GetError());
            }
        }

        ~impl()
        {
            SDL_GL_DeleteContext(gl_context);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }

        void update(game_time delta_time)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                    exit();
                }
            }
        }
    };
    
    window::window(const std::string& title)
    : impl_(new impl(title))
    {
        impl_->exit.connect([this]()
        {
            exit();
        });
    }

    window::~window() = default;

    void window::end_frame(game_time delta_time) const
    {
        impl_->end_frame(delta_time);
    }

    void window::update(game_time delta_time) const
    {
        impl_->update(delta_time);
    }
}
