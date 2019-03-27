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
#include <graphics/buffer/frame_buffer_handle.hpp>
#include <graphics/device/graphics_device.hpp>

namespace moka
{
    bool frame_buffer_handle::operator==(const frame_buffer_handle& rhs) const
    {
        return id == rhs.id;
    }

    bool frame_buffer_handle::operator!=(const frame_buffer_handle& rhs) const
    {
        return id != rhs.id;
    }

    bool frame_buffer_handle::operator>(const frame_buffer_handle& rhs) const
    {
        return id > rhs.id;
    }

    bool frame_buffer_handle::operator<(const frame_buffer_handle& rhs) const
    {
        return id < rhs.id;
    }

    bool frame_buffer_handle::operator>=(const frame_buffer_handle& rhs) const
    {
        return id >= rhs.id;
    }

    bool frame_buffer_handle::operator<=(const frame_buffer_handle& rhs) const
    {
        return id <= rhs.id;
    }

    frame_buffer_builder::frame_buffer_builder(graphics_device& device)
        : device_(device)
    {
    }

    frame_buffer_builder& frame_buffer_builder::add_depth_attachment(
        const frame_format format, const int width, const int height)
    {
        render_attachments.emplace_back(
            render_texture_data{frame_attachment::depth, format, width, height});
        return *this;
    }

    frame_buffer_builder& frame_buffer_builder::add_color_attachment(
        const frame_format format, const int width, const int height)
    {
        render_attachments.emplace_back(
            render_texture_data{frame_attachment::color, format, width, height});
        return *this;
    }

    frame_buffer_builder& frame_buffer_builder::add_stencil_attachment(
        const frame_format format, const int width, const int height)
    {
        render_attachments.emplace_back(render_texture_data{
            frame_attachment::stencil, format, width, height});
        return *this;
    }

    frame_buffer_builder& frame_buffer_builder::add_depth_stencil_attachment(
        const frame_format format, const int width, const int height)
    {
        render_attachments.emplace_back(render_texture_data{
            frame_attachment::depth_stencil, format, width, height});
        return *this;
    }

    frame_buffer_handle frame_buffer_builder::build()
    {
        return device_.make_frame_buffer(
            render_attachments.data(), render_attachments.size());
    }
} // namespace moka
