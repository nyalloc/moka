/*
===========================================================================
Moka Source Code
Copyright 2019 Stuart Adams. All rights reserved.
https://github.com/stuartdadams/moka
stuartdadams | linkedin.com/in/stuartdadams

This file is part of the Moka Real-Time Physically-Based Rendering Project.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

===========================================================================
*/
#include <SDL.h>
#include <application/application.hpp>
#include <iostream>

namespace moka
{
    constexpr key sdl_to_moka(int key)
    {
        switch (key)
        {
        case SDLK_BACKSLASH:
            return key::backslash;
        case SDLK_BACKSPACE:
            return key::backspace;
        case SDLK_SPACE:
            return key::space;
        case SDLK_COMMA:
            return key::comma;
        case SDLK_DELETE:
            return key::del;
        case SDLK_DOWN:
            return key::down;
        case SDLK_UP:
            return key::up;
        case SDLK_LEFT:
            return key::left;
        case SDLK_RIGHT:
            return key::right;
        case SDLK_END:
            return key::end;
        case SDLK_RETURN:
            return key::enter;
        case SDLK_ESCAPE:
            return key::esc;
        case SDLK_F1:
            return key::f1;
        case SDLK_F2:
            return key::f2;
        case SDLK_F3:
            return key::f3;
        case SDLK_F4:
            return key::f4;
        case SDLK_F5:
            return key::f5;
        case SDLK_F6:
            return key::f6;
        case SDLK_F7:
            return key::f7;
        case SDLK_F8:
            return key::f8;
        case SDLK_F9:
            return key::f9;
        case SDLK_F10:
            return key::f10;
        case SDLK_F11:
            return key::f11;
        case SDLK_F12:
            return key::f12;
        case SDLK_a:
            return key::a;
        case SDLK_b:
            return key::b;
        case SDLK_c:
            return key::c;
        case SDLK_d:
            return key::d;
        case SDLK_e:
            return key::e;
        case SDLK_f:
            return key::f;
        case SDLK_g:
            return key::g;
        case SDLK_h:
            return key::h;
        case SDLK_i:
            return key::i;
        case SDLK_j:
            return key::j;
        case SDLK_k:
            return key::k;
        case SDLK_l:
            return key::l;
        case SDLK_m:
            return key::m;
        case SDLK_n:
            return key::n;
        case SDLK_o:
            return key::o;
        case SDLK_p:
            return key::p;
        case SDLK_q:
            return key::q;
        case SDLK_r:
            return key::r;
        case SDLK_s:
            return key::s;
        case SDLK_t:
            return key::t;
        case SDLK_u:
            return key::u;
        case SDLK_v:
            return key::v;
        case SDLK_w:
            return key::w;
        case SDLK_x:
            return key::x;
        case SDLK_y:
            return key::y;
        case SDLK_z:
            return key::z;
        default:
            return key::none;
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
                window_.exit();
                break;
            }
            case SDL_MOUSEMOTION:
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                mouse_.state_.position_ = glm::ivec2{x, y};
                mouse_.state_.motion_ =
                    glm::ivec2{event.motion.xrel, event.motion.yrel};

                log_.debug("SDL_MOUSEMOTION");
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                switch (event.button.button)
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
                default:;
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

                mouse_.state_.scroll_.x = event.wheel.x;
                mouse_.state_.scroll_.y = event.wheel.y;
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
                keyboard_.state_.set_key_down(sdl_to_moka(event.key.keysym.sym));
                break;
            }
            case SDL_KEYUP:
            {
                log_.debug("SDL_KEYUP");
                keyboard_.state_.set_key_up(sdl_to_moka(event.key.keysym.sym));
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
} // namespace moka
