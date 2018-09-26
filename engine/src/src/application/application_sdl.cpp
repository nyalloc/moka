#include <application/application.hpp>
#include <SDL.h>

namespace moka
{
	constexpr key sdl_to_moka(int key)
	{
		switch (key)
		{
		case SDLK_BACKSLASH: return key::backslash;
		case SDLK_BACKSPACE: return key::backspace;
		case SDLK_SPACE: return key::space;
		case SDLK_COMMA: return key::comma;
		case SDLK_DELETE: return key::del;
		case SDLK_DOWN: return key::down;
		case SDLK_END: return key::end;
		case SDLK_RETURN: return key::enter;
		case SDLK_ESCAPE: return key::esc;
		case SDLK_F1: return key::f1;
		case SDLK_F2: return key::f2;
		case SDLK_F3: return key::f3;
		case SDLK_F4: return key::f4;
		case SDLK_F5: return key::f5;
		case SDLK_F6: return key::f6;
		case SDLK_F7: return key::f7;
		case SDLK_F8: return key::f8;
		case SDLK_F9: return key::f9;
		case SDLK_F10: return key::f10;
		case SDLK_F11: return key::f11;
		case SDLK_F12: return key::f12;

		case SDLK_w: return key::key_w;
		case SDLK_a: return key::key_a;
		case SDLK_s: return key::key_s;
		case SDLK_d: return key::key_d;

		default: return key::none;
		}
	}

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
				keyboard_.state.set_key_down(sdl_to_moka(event.key.keysym.sym));
				break;
			}
			case SDL_KEYUP:
			{
				log_.debug("SDL_KEYUP");
				keyboard_.state.set_key_up(sdl_to_moka(event.key.keysym.sym));
				break;
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
			log_.debug("Updating application. Delta time: {}", delta_time);
			poll_events();
			// pre update
			update(delta_time);
			// post update
		};

		auto draw_app = [&](const game_time delta_time)
		{
			log_.debug("Rendering application. Delta time: {}", delta_time);
			// pre draw
			draw(delta_time);
			// post draw
		};

		double t = 0.0;
		const double dt = 0.01;

		double currentTime = elapsed();
		double accumulator = 0.0;

		while (running_)
		{
			double newTime = double(elapsed());
			double frameTime = newTime - currentTime;
			currentTime = newTime;

			accumulator += frameTime;

			while (accumulator >= dt)
			{
				update_app(game_time(dt));
				accumulator -= dt;
				t += dt;
			}

			draw_app(game_time(dt));
		}

        return 0;
    }
}
