#include <application/application.hpp>
#include <SDL.h>

namespace moka
{
    void sdl_events(application& app, logger& log)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
            {
                log.log(level::debug, "SDL_QUIT");
                break;
            }
            case SDL_MOUSEMOTION:
            {
                log.log(level::debug, "SDL_MOUSEMOTION");
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                log.log(level::debug, "SDL_MOUSEBUTTONDOWN");
                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                log.log(level::debug, "SDL_MOUSEBUTTONDOWN");
                break;
            }
            case SDL_MOUSEWHEEL:
            {
                log.log(level::debug, "SDL_MOUSEWHEEL");
                break;
            }
            case SDL_TEXTINPUT:
            {
                log.log(level::debug, "SDL_TEXTINPUT");
                break;
            }
            case SDL_KEYDOWN:
            {
                log.log(level::debug, "SDL_KEYDOWN");

                switch (event.key.keysym.sym)
                {
                case SDLK_LEFT:
                    log.log(level::debug, "SDLK_LEFT");
                    break;
                case SDLK_RIGHT:
                    log.log(level::debug, "SDLK_RIGHT");
                    break;
                case SDLK_UP:
                    log.log(level::debug, "SDLK_UP");
                    break;
                case SDLK_DOWN:
                    log.log(level::debug, "SDLK_DOWN");
                    break;
                default:
                    break;
                }
            }
            case SDL_KEYUP:
            {
                log.log(level::debug, "SDL_KEYUP");

                switch (event.key.keysym.sym)
                {
                case SDLK_LEFT:
                    log.log(level::debug, "SDLK_LEFT");
                    break;
                case SDLK_RIGHT:
                    log.log(level::debug, "SDLK_RIGHT");
                    break;
                case SDLK_UP:
                    log.log(level::debug, "SDLK_UP");
                    break;
                case SDLK_DOWN:
                    log.log(level::debug, "SDLK_DOWN");
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
                        log.log(level::debug, "SDL_WINDOWEVENT_SHOWN");
                        break;
                    case SDL_WINDOWEVENT_HIDDEN:
                        log.log(level::debug, "SDL_WINDOWEVENT_HIDDEN");
                        break;
                    case SDL_WINDOWEVENT_EXPOSED:
                        log.log(level::debug, "SDL_WINDOWEVENT_EXPOSED");
                        break;
                    case SDL_WINDOWEVENT_MOVED:
                        log.log(level::debug, "SDL_WINDOWEVENT_MOVED");
                        break;
                    case SDL_WINDOWEVENT_RESIZED:
                        log.log(level::debug, "SDL_WINDOWEVENT_RESIZED");
                        break;
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        log.log(level::debug, "SDL_WINDOWEVENT_SIZE_CHANGED");
                        break;
                    case SDL_WINDOWEVENT_MINIMIZED:
                        log.log(level::debug, "SDL_WINDOWEVENT_MINIMIZED");
                        break;
                    case SDL_WINDOWEVENT_MAXIMIZED:
                        log.log(level::debug, "SDL_WINDOWEVENT_MAXIMIZED");
                        break;
                    case SDL_WINDOWEVENT_RESTORED:
                        log.log(level::debug, "SDL_WINDOWEVENT_RESTORED");
                        break;
                    case SDL_WINDOWEVENT_ENTER:
                        log.log(level::debug, "SDL_WINDOWEVENT_ENTER");
                        break;
                    case SDL_WINDOWEVENT_LEAVE:
                        log.log(level::debug, "SDL_WINDOWEVENT_LEAVE");
                        break;
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                        log.log(level::debug, "SDL_WINDOWEVENT_FOCUS_GAINED");
                        break;
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                        log.log(level::debug, "SDL_WINDOWEVENT_FOCUS_LOST");
                        break;
                    case SDL_WINDOWEVENT_CLOSE:
                        log.log(level::debug, "SDL_WINDOWEVENT_CLOSE");
                        break;
                    case SDL_WINDOWEVENT_TAKE_FOCUS:
                        log.log(level::debug, "SDL_WINDOWEVENT_TAKE_FOCUS");
                        break;
                    case SDL_WINDOWEVENT_HIT_TEST:
                        log.log(level::debug, "SDL_WINDOWEVENT_HIT_TEST");
                        break;
                    default:
                        log.log(level::debug, "UNKNOWN SDL WINDOW EVENT");
                        break;
                    }
                }
                break;
            }
            case SDL_JOYAXISMOTION:
            {
                log.log(level::debug, "SDL_JOYAXISMOTION");
                break;
            }
            case SDL_CONTROLLERAXISMOTION:
            {
                log.log(level::debug, "SDL_CONTROLLERAXISMOTION");
                break;
            }
            case SDL_JOYBUTTONDOWN:
            {
                log.log(level::debug, "SDL_JOYBUTTONDOWN");
                break;
            }
            case SDL_JOYBUTTONUP:
            {
                log.log(level::debug, "SDL_JOYBUTTONUP");
                break;
            }
            case SDL_CONTROLLERBUTTONDOWN:
            {
                log.log(level::debug, "SDL_CONTROLLERBUTTONDOWN");
                break;
            }
            case SDL_CONTROLLERBUTTONUP:
            {
                log.log(level::debug, "SDL_CONTROLLERBUTTONUP");
                break;
            }
            case SDL_JOYDEVICEADDED:
            {
                log.log(level::debug, "SDL_JOYDEVICEADDED");
                break;
            }
            case SDL_JOYDEVICEREMOVED:
            {
                log.log(level::debug, "SDL_JOYDEVICEREMOVED");
                break;
            }
            case SDL_CONTROLLERDEVICEADDED:
            {
                log.log(level::debug, "SDL_CONTROLLERDEVICEADDED");
                break;
            }
            case SDL_CONTROLLERDEVICEREMAPPED:
            {
                log.log(level::debug, "SDL_CONTROLLERDEVICEREMAPPED");
                break;
            }
            case SDL_CONTROLLERDEVICEREMOVED:
            {
                log.log(level::debug, "SDL_CONTROLLERDEVICEREMOVED");
                break;
            }
            case SDL_DROPFILE:
            {
                log.log(level::debug, "SDL_DROPFILE");
                break;
            }
            default: ;
            }
        }
    }

    int application::run()
    {
        dispatcher_.run();

        window_.exit.connect([this]()
        {
            log_.log(level::info, "Exiting application");
            running_ = false;
        });

        while (running_)
        {
            sdl_events(*this, log_);
            update(16.0f);
            draw(16.0f);
        }

        return 0;
    }

    filesystem::path application::data_path()
    {
        return filesystem::current_path();
    }
}
