#include <window/window.hpp>
#include <GL/glew.h>
#include <SDL.h>
#include <logger/logger.hpp>
#include <atomic>

namespace moka
{
    class window::impl
    {
        SDL_Window* window_;
        bool running_;
        logger log_{ filesystem::current_path() / "window.log" };
		std::unordered_map<handle, SDL_GLContext> contexts_;
    public:
        signal<> exit;

        void swap_buffer() const;

        explicit impl(const std::string& title);

        ~impl();

        void set_size(int width, int height);

		context_handle make_context();

		void make_current(const context_handle handle);
    };

	context_handle window::impl::make_context()
	{
		static std::atomic<handle> current_id;
		contexts_.emplace(current_id, SDL_GL_CreateContext(window_));
		const context_handle handle{ current_id };
		++current_id;
		return handle;
	}

	void window::impl::make_current(const context_handle handle)
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

    window::impl::impl(const std::string& title): running_(true)
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

		window_ = SDL_CreateWindow("Moka", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL);
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

    window::window(const std::string& title)
    : impl_(new impl(title))
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

	void window::make_current(const context_handle handle)
	{
		impl_->make_current(handle);
	}
}
