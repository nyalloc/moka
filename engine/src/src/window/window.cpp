#include <window/window.hpp>
#include <GL/glew.h>
#include <SDL.h>
#include <logger/logger.hpp>

namespace neon
{
    class window::impl
    {
        SDL_Window* window_;
        SDL_GLContext context_;
        bool running_;
        logger log_{ filesystem::current_path() / "window.log" };
    public:
        signal<> exit;

        void end_frame(game_time delta_time) const;

        explicit impl(const std::string& title);

        ~impl();

        void update(game_time delta_time);

        void set_size(int width, int height);
    };

    void window::impl::end_frame(const game_time delta_time) const
    {
        SDL_GL_SwapWindow(window_);
    }

    window::impl::impl(const std::string& title): running_(true)
    {
        log_.log(level::info, "Initializing main Window, creating OpenGL context.");

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

            window_ = SDL_CreateWindow(
                title.c_str(), 
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED, 
                1280,
                720,                          
                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

            context_ = SDL_GL_CreateContext(window_);

            SDL_GL_SetSwapInterval(1); // Enable vsync

            glewExperimental = GL_TRUE;
            if (glewInit() != GLEW_OK)
            {
                log_.log(level::fatal, "Failed to initialize GLEW");
                std::abort();
            }
        }
        else
        {
            log_.log(level::error, SDL_GetError());
        }
    }

    window::impl::~impl()
    {
        SDL_GL_DeleteContext(context_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

    void window::impl::update(const game_time delta_time)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running_ = false;
                exit();
            }
        }
    }

    void window::impl::set_size(const int width, const int height)
    {
        SDL_SetWindowSize(window_, width, height);
    }

    window::window(const std::string& title)
    : impl_(new impl(title))
    {
        impl_->exit.connect([this]()
        {
            exit();
        });
    }

    window::~window() = default;

    void window::end_frame(const game_time delta_time) const
    {
        impl_->end_frame(delta_time);
    }

    void window::update(const game_time delta_time) const
    {
        impl_->update(delta_time);
    }

    void window::set_size(const int width, const int height)
    {
        impl_->set_size(width, height);
    }
}
