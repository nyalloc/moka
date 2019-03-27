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
#include <input/mouse.hpp>

namespace moka
{
    const glm::ivec2& mouse_state::get_position() const
    {
        return position_;
    }

    const glm::ivec2& mouse_state::get_motion() const
    {
        return motion_;
    }

    bool mouse_state::is_button_down(const mouse_button button) const
    {
        const auto pos = static_cast<size_t>(button);
        return buttons_[pos];
    }

    bool mouse_state::is_button_up(const mouse_button button) const
    {
        return !is_button_down(button);
    }

    glm::ivec2 mouse_state::get_scroll() const
    {
        return scroll_;
    }

    const mouse_state& mouse::get_state() const
    {
        return state_;
    }
} // namespace moka
