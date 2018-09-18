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
				log_.debug("SDL_QUIT");
				break;
			}
			case SDL_MOUSEMOTION:
			{
				log_.debug("SDL_MOUSEMOTION");
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				log_.debug("SDL_MOUSEBUTTONDOWN");
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				log_.debug("SDL_MOUSEBUTTONUP");
				break;
			}
			case SDL_MOUSEWHEEL:
			{
				log_.debug("SDL_MOUSEWHEEL");
				break;
			}
			case SDL_TEXTINPUT:
			{
				log_.debug("SDL_TEXTINPUT");
				break;
			}
			case SDL_KEYDOWN:
			{
				log_.debug("SDL_KEYDOWN");
				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
					break;
				case SDLK_RIGHT:
					break;
				case SDLK_UP:
					break;
				case SDLK_DOWN:
					break;
				default:
					break;
				}
			}
			case SDL_KEYUP:
			{
				log_.debug("SDL_KEYUP");
				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
					break;
				case SDLK_RIGHT:
					break;
				case SDLK_UP:
					break;
				case SDLK_DOWN:
					break;
				default:
					break;
				}
			}
			case SDL_WINDOWEVENT:
			{
				log_.debug("SDL_WINDOWEVENT");
				if (event.type == SDL_WINDOWEVENT)
				{
					switch (event.window.event)
					{
					case SDL_WINDOWEVENT_SHOWN:
						break;
					case SDL_WINDOWEVENT_HIDDEN:
						break;
					case SDL_WINDOWEVENT_EXPOSED:
						break;
					case SDL_WINDOWEVENT_MOVED:
						break;
					case SDL_WINDOWEVENT_RESIZED:
						break;
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						break;
					case SDL_WINDOWEVENT_MINIMIZED:
						break;
					case SDL_WINDOWEVENT_MAXIMIZED:
						break;
					case SDL_WINDOWEVENT_RESTORED:
						break;
					case SDL_WINDOWEVENT_ENTER:
						break;
					case SDL_WINDOWEVENT_LEAVE:
						break;
					case SDL_WINDOWEVENT_FOCUS_GAINED:
						break;
					case SDL_WINDOWEVENT_FOCUS_LOST:
						break;
					case SDL_WINDOWEVENT_CLOSE:
						break;
					case SDL_WINDOWEVENT_TAKE_FOCUS:
						break;
					case SDL_WINDOWEVENT_HIT_TEST:
						break;
					default:
						break;
					}
				}
				break;
			}
			case SDL_JOYAXISMOTION:
			{
				log_.debug("SDL_JOYAXISMOTION");
				break;
			}
			case SDL_CONTROLLERAXISMOTION:
			{
				log_.debug("SDL_CONTROLLERAXISMOTION");
				break;
			}
			case SDL_JOYBUTTONDOWN:
			{
				log_.debug("SDL_JOYBUTTONDOWN");
				break;
			}
			case SDL_JOYBUTTONUP:
			{
				log_.debug("SDL_JOYBUTTONUP");
				break;
			}
			case SDL_CONTROLLERBUTTONDOWN:
			{
				log_.debug("SDL_CONTROLLERBUTTONDOWN");
				break;
			}
			case SDL_CONTROLLERBUTTONUP:
			{
				log_.debug("SDL_CONTROLLERBUTTONUP");
				break;
			}
			case SDL_JOYDEVICEADDED:
			{
				log_.debug("SDL_JOYDEVICEADDED");
				break;
			}
			case SDL_JOYDEVICEREMOVED:
			{
				log_.debug("SDL_JOYDEVICEREMOVED");
				break;
			}
			case SDL_CONTROLLERDEVICEADDED:
			{
				log_.debug("SDL_CONTROLLERDEVICEADDED");
				break;
			}
			case SDL_CONTROLLERDEVICEREMAPPED:
			{
				log_.debug("SDL_CONTROLLERDEVICEREMAPPED");
				break;
			}
			case SDL_CONTROLLERDEVICEREMOVED:
			{
				log_.debug("SDL_CONTROLLERDEVICEREMOVED");
				break;
			}
			case SDL_DROPFILE:
			{
				log_.debug("SDL_DROPFILE");
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
			log_.info("Exiting application");
            running_ = false;
        });

		auto update_app = [&](const game_time delta_time)
		{
			log_.debug("Updating application");
			poll_events();
			// pre update
			update(delta_time);
			// post update
		};

		auto draw_app = [&](const game_time delta_time)
		{
			log_.debug("Rendering application");
			// pre draw
			draw(delta_time);
			// post draw
		};

		while (running_)
		{
			update_app(16.0f);
			draw_app(16.0f);
		}

        return 0;
    }
}
