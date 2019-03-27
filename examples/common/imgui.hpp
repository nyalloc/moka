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

#pragma once

#include <application/logger.hpp>
#include <graphics/device/graphics_device.hpp>
#include <graphics/material/material.hpp>
#include <imgui.h>

namespace moka
{
    class keyboard;
    class mouse;

    class imgui
    {
        window& window_;
        keyboard& keyboard_;
        mouse& mouse_;

        logger log_;

        material_handle material_;

        index_buffer_handle index_buffer_;
        vertex_buffer_handle vertex_buffer_;

        graphics_device& graphics_device_;

        texture_handle font_atlas_{};

    public:
        imgui(window& window, keyboard& keyboard, mouse& mouse, graphics_device& graphics_device);

        void new_frame(float delta_time) const;

        command_list draw() const;
    };
} // namespace moka
