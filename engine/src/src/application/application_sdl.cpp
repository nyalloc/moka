#include <application/application.hpp>
#include <SDL.h>
#include "application/profile.hpp"

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
		case SDLK_UP: return key::up;
		case SDLK_LEFT: return key::left;
		case SDLK_RIGHT: return key::right;
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
		case SDLK_a: return key::a;
		case SDLK_b: return key::b;
		case SDLK_c: return key::c;
		case SDLK_d: return key::d;
		case SDLK_e: return key::e;
		case SDLK_f: return key::f;
		case SDLK_g: return key::g;
		case SDLK_h: return key::h;
		case SDLK_i: return key::i;
		case SDLK_j: return key::j;
		case SDLK_k: return key::k;
		case SDLK_l: return key::l;
		case SDLK_m: return key::m;
		case SDLK_n: return key::n;
		case SDLK_o: return key::o;
		case SDLK_p: return key::p;
		case SDLK_q: return key::q;
		case SDLK_r: return key::r;
		case SDLK_s: return key::s;
		case SDLK_t: return key::t;
		case SDLK_u: return key::u;
		case SDLK_v: return key::v;
		case SDLK_w: return key::w;
		case SDLK_x: return key::x;
		case SDLK_y: return key::y;
		case SDLK_z: return key::z;
		default: return key::none;
		}
	}

	void app::poll_events()
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
				int x, y;
				SDL_GetMouseState(&x, &y);
				mouse_.state_.position_ = glm::ivec2{ x, y };
				mouse_.state_.motion_ = glm::ivec2{ event.motion.xrel, event.motion.yrel };

				log_.debug("SDL_MOUSEMOTION");
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				switch(event.button.button)
				{
				case SDL_BUTTON_LEFT:
					mouse_.state_.buttons_[static_cast<size_t>(mouse_button::left)] = true;
					break;
				case SDL_BUTTON_RIGHT:
					mouse_.state_.buttons_[static_cast<size_t>(mouse_button::right)] = true;
					break;
				case SDL_BUTTON_MIDDLE:
					mouse_.state_.buttons_[static_cast<size_t>(mouse_button::middle)] = true;
					break;
				default: ;
				}

				log_.debug("SDL_MOUSEBUTTONDOWN");
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
					mouse_.state_.buttons_[static_cast<size_t>(mouse_button::left)] = false;
					break;
				case SDL_BUTTON_RIGHT:
					mouse_.state_.buttons_[static_cast<size_t>(mouse_button::right)] = false;
					break;
				case SDL_BUTTON_MIDDLE:
					mouse_.state_.buttons_[static_cast<size_t>(mouse_button::middle)] = false;
					break;
				default:;
				}

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

    int app::run()
    {
		try
		{
			window_.exit.connect([this]()
			{
				log_.info("Exiting application");
				running_ = false;
			});

			float t = 0.0f;
			const float fixed_update_time = 1.0f / 60.0f;

			float current_time = elapsed();
			float delta_time = 0.0f;

			while (running_)
			{
				float new_time = elapsed();
				float frame_time = new_time - current_time;
				current_time = new_time;

				delta_time += frame_time;

				while (delta_time >= fixed_update_time)
				{
					mouse_.state_.motion_ = { 0, 0 };

					poll_events();
					update(fixed_update_time);

					delta_time -= fixed_update_time;
					t += fixed_update_time;
				}

				const auto duration = profile<milliseconds>([&]()
				{
					if (delta_time > 1.0f / 60.0f)
					{
						std::cout << "Frame rate dropped bellow 60. Delta time: " << fixed_update_time << "\n";
					}

					draw(delta_time);
				});

				//std::cout << "Total draw time: " << duration << " ms" << std::endl;
			}
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}

        return 0;
    }
}
