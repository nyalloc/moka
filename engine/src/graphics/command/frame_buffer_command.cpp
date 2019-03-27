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
#include <graphics/command/frame_buffer_command.hpp>
#include <graphics/device/graphics_visitor.hpp>

namespace moka
{
    frame_buffer_command::frame_buffer_command() = default;

    frame_buffer_command::~frame_buffer_command() = default;

    frame_buffer_command::frame_buffer_command(const frame_buffer_command& rhs)
        : buffer(rhs.buffer)
    {
    }

    frame_buffer_command& frame_buffer_command::operator=(const frame_buffer_command& rhs)
    {
        buffer = rhs.buffer;
        return *this;
    }

    frame_buffer_command::frame_buffer_command(frame_buffer_command&& rhs) noexcept
        : buffer(rhs.buffer)
    {
    }

    frame_buffer_command& frame_buffer_command::operator=(frame_buffer_command&& rhs) noexcept
    {
        buffer = rhs.buffer;
        return *this;
    }

    void frame_buffer_command::accept(graphics_visitor& visitor)
    {
        visitor.visit(*this);
    }

    frame_buffer_command& frame_buffer_command::set_frame_buffer(const frame_buffer_handle buffer)
    {
        this->buffer = buffer;
        return *this;
    }
} // namespace moka
