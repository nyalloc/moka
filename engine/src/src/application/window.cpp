#include <application/window.hpp>
#include <GL/glew.h>
#include <SDL.h>
#include <application/logger.hpp>
#include <atomic>
#include <iostream>
#include <string>

namespace moka
{
    class window::impl
    {
        SDL_Window* window_;
        bool running_;
		logger log_{ "Window" };
		std::unordered_map<handle_id, SDL_GLContext> contexts_;
    public:
        signal<> exit;

        void swap_buffer() const;

        explicit impl(const window_settings& settings);

        ~impl();

        void set_size(int width, int height);

		context_handle make_context();

		void set_current_context(const context_handle handle);
    };

	context_handle window::impl::make_context()
	{
		static std::atomic<handle_id> current_id = 0;
		contexts_.emplace(current_id, SDL_GL_CreateContext(window_));
		const context_handle handle{ current_id };
		++current_id;
		return handle;
	}

	void window::impl::set_current_context(const context_handle handle)
	{
		const auto pos = contexts_.find(handle.id);
		if(pos != contexts_.end())
		{
			SDL_GL_MakeCurrent(window_, contexts_[handle.id]);
		}
	}

    void window::impl::swap_buffer() const
    {
        SDL_GL_SwapWindow(window_);
    }

    window::impl::impl(const window_settings& settings)
		: running_(true)
    {
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cout << "REEEE";
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

		window_ = SDL_CreateWindow("Moka", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 720, 720, SDL_WINDOW_OPENGL);
    }

    window::impl::~impl()
    {
		for(const auto& item : contexts_)
		{
			SDL_GL_DeleteContext(item.second);
		}
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

    void window::impl::set_size(const int width, const int height)
    {
        SDL_SetWindowSize(window_, width, height);
    }

    window::window(const window_settings& settings)
    : impl_(new impl(settings))
    {
        impl_->exit.connect([this]()
        {
            exit();
        });
    }

    window::~window() = default;

    void window::swap_buffer() const
    {
        impl_->swap_buffer();
    }

    void window::set_size(const int width, const int height)
    {
        impl_->set_size(width, height);
    }

	context_handle window::make_context() const
	{
		return impl_->make_context();
	}

	void window::set_current_context(const context_handle handle)
	{
		impl_->set_current_context(handle);
	}
}
