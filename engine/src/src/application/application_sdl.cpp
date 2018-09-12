#include <application/application.hpp>
#include <SDL.h>

namespace moka
{
	void application::poll_events()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
			{
				log_.log(level::debug, "SDL_QUIT");
				break;
			}
			case SDL_MOUSEMOTION:
			{
				log_.log(level::debug, "SDL_MOUSEMOTION");
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				log_.log(level::debug, "SDL_MOUSEBUTTONDOWN");
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				log_.log(level::debug, "SDL_MOUSEBUTTONDOWN");
				break;
			}
			case SDL_MOUSEWHEEL:
			{
				log_.log(level::debug, "SDL_MOUSEWHEEL");
				break;
			}
			case SDL_TEXTINPUT:
			{
				log_.log(level::debug, "SDL_TEXTINPUT");
				break;
			}
			case SDL_KEYDOWN:
			{
				log_.log(level::debug, "SDL_KEYDOWN");

				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
					log_.log(level::debug, "SDLK_LEFT");
					break;
				case SDLK_RIGHT:
					log_.log(level::debug, "SDLK_RIGHT");
					break;
				case SDLK_UP:
					log_.log(level::debug, "SDLK_UP");
					break;
				case SDLK_DOWN:
					log_.log(level::debug, "SDLK_DOWN");
					break;
				default:
					break;
				}
			}
			case SDL_KEYUP:
			{
				log_.log(level::debug, "SDL_KEYUP");

				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
					log_.log(level::debug, "SDLK_LEFT");
					break;
				case SDLK_RIGHT:
					log_.log(level::debug, "SDLK_RIGHT");
					break;
				case SDLK_UP:
					log_.log(level::debug, "SDLK_UP");
					break;
				case SDLK_DOWN:
					log_.log(level::debug, "SDLK_DOWN");
					break;
				default:
					break;
				}
			}
			case SDL_WINDOWEVENT:
			{
				if (event.type == SDL_WINDOWEVENT)
				{
					switch (event.window.event)
					{
					case SDL_WINDOWEVENT_SHOWN:
						log_.log(level::debug, "SDL_WINDOWEVENT_SHOWN");
						break;
					case SDL_WINDOWEVENT_HIDDEN:
						log_.log(level::debug, "SDL_WINDOWEVENT_HIDDEN");
						break;
					case SDL_WINDOWEVENT_EXPOSED:
						log_.log(level::debug, "SDL_WINDOWEVENT_EXPOSED");
						break;
					case SDL_WINDOWEVENT_MOVED:
						log_.log(level::debug, "SDL_WINDOWEVENT_MOVED");
						break;
					case SDL_WINDOWEVENT_RESIZED:
						log_.log(level::debug, "SDL_WINDOWEVENT_RESIZED");
						break;
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						log_.log(level::debug, "SDL_WINDOWEVENT_SIZE_CHANGED");
						break;
					case SDL_WINDOWEVENT_MINIMIZED:
						log_.log(level::debug, "SDL_WINDOWEVENT_MINIMIZED");
						break;
					case SDL_WINDOWEVENT_MAXIMIZED:
						log_.log(level::debug, "SDL_WINDOWEVENT_MAXIMIZED");
						break;
					case SDL_WINDOWEVENT_RESTORED:
						log_.log(level::debug, "SDL_WINDOWEVENT_RESTORED");
						break;
					case SDL_WINDOWEVENT_ENTER:
						log_.log(level::debug, "SDL_WINDOWEVENT_ENTER");
						break;
					case SDL_WINDOWEVENT_LEAVE:
						log_.log(level::debug, "SDL_WINDOWEVENT_LEAVE");
						break;
					case SDL_WINDOWEVENT_FOCUS_GAINED:
						log_.log(level::debug, "SDL_WINDOWEVENT_FOCUS_GAINED");
						break;
					case SDL_WINDOWEVENT_FOCUS_LOST:
						log_.log(level::debug, "SDL_WINDOWEVENT_FOCUS_LOST");
						break;
					case SDL_WINDOWEVENT_CLOSE:
						log_.log(level::debug, "SDL_WINDOWEVENT_CLOSE");
						break;
					case SDL_WINDOWEVENT_TAKE_FOCUS:
						log_.log(level::debug, "SDL_WINDOWEVENT_TAKE_FOCUS");
						break;
					case SDL_WINDOWEVENT_HIT_TEST:
						log_.log(level::debug, "SDL_WINDOWEVENT_HIT_TEST");
						break;
					default:
						log_.log(level::debug, "UNKNOWN SDL WINDOW EVENT");
						break;
					}
				}
				break;
			}
			case SDL_JOYAXISMOTION:
			{
				log_.log(level::debug, "SDL_JOYAXISMOTION");
				break;
			}
			case SDL_CONTROLLERAXISMOTION:
			{
				log_.log(level::debug, "SDL_CONTROLLERAXISMOTION");
				break;
			}
			case SDL_JOYBUTTONDOWN:
			{
				log_.log(level::debug, "SDL_JOYBUTTONDOWN");
				break;
			}
			case SDL_JOYBUTTONUP:
			{
				log_.log(level::debug, "SDL_JOYBUTTONUP");
				break;
			}
			case SDL_CONTROLLERBUTTONDOWN:
			{
				log_.log(level::debug, "SDL_CONTROLLERBUTTONDOWN");
				break;
			}
			case SDL_CONTROLLERBUTTONUP:
			{
				log_.log(level::debug, "SDL_CONTROLLERBUTTONUP");
				break;
			}
			case SDL_JOYDEVICEADDED:
			{
				log_.log(level::debug, "SDL_JOYDEVICEADDED");
				break;
			}
			case SDL_JOYDEVICEREMOVED:
			{
				log_.log(level::debug, "SDL_JOYDEVICEREMOVED");
				break;
			}
			case SDL_CONTROLLERDEVICEADDED:
			{
				log_.log(level::debug, "SDL_CONTROLLERDEVICEADDED");
				break;
			}
			case SDL_CONTROLLERDEVICEREMAPPED:
			{
				log_.log(level::debug, "SDL_CONTROLLERDEVICEREMAPPED");
				break;
			}
			case SDL_CONTROLLERDEVICEREMOVED:
			{
				log_.log(level::debug, "SDL_CONTROLLERDEVICEREMOVED");
				break;
			}
			case SDL_DROPFILE:
			{
				log_.log(level::debug, "SDL_DROPFILE");
				break;
			}
			default:;
			}
		}
	}

    int application::run()
    {
        window_.exit.connect([this]()
        {
            log_.log(level::info, "Exiting application");
            running_ = false;
        });

		auto update_app = [&](const game_time delta_time)
		{
			// pre update
			update(16.0f);
			// post update
		};

		auto draw_app = [&](const game_time delta_time)
		{
			// pre draw
			draw(16.0f);
			// post draw
		};

        while (running_)
        {
            poll_events();
			update_app(16.0f);
			draw_app(16.0f);
        }

        return 0;
    }

    filesystem::path application::data_path()
    {
        return filesystem::current_path();
    }
}
