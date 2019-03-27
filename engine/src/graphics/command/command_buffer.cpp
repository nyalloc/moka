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
#include <graphics/command/clear_command.hpp>
#include <graphics/command/command_buffer.hpp>
#include <graphics/command/draw_command.hpp>
#include <graphics/command/fill_index_buffer_command.hpp>
#include <graphics/command/fill_vertex_buffer_command.hpp>
#include <graphics/command/frame_buffer_command.hpp>
#include <graphics/command/frame_buffer_texture_command.hpp>
#include <graphics/command/generate_mipmaps_command.hpp>
#include <graphics/command/scissor_command.hpp>
#include <graphics/command/set_material_properties_command.hpp>
#include <graphics/command/viewport_command.hpp>

namespace moka
{
    command_buffer::command_buffer(const sort_key id) : id_(id)
    {
    }

    command_buffer::command_buffer(command_buffer&& command_buffer) noexcept
        : commands_(std::move(command_buffer.commands_)), id_(command_buffer.id_)
    {
    }

    command_buffer& command_buffer::operator=(command_buffer&& command_buffer) noexcept
    {
        commands_ = std::move(command_buffer.commands_);
        id_ = command_buffer.id_;
        return *this;
    }

    const sort_key& command_buffer::get_key() const
    {
        return id_;
    }

    void command_buffer::accept(graphics_visitor& device)
    {
        for (auto& command : commands_)
        {
            command->accept(device);
        }
    }

    frame_buffer_command& command_buffer::frame_buffer()
    {
        return emplace_back<frame_buffer_command>();
    }

    frame_buffer_texture_command& command_buffer::frame_buffer_texture()
    {
        return emplace_back<frame_buffer_texture_command>();
    }

    clear_command& command_buffer::clear()
    {
        return emplace_back<clear_command>();
    }

    draw_command& command_buffer::draw()
    {
        return emplace_back<draw_command>();
    }

    scissor_command& command_buffer::scissor()
    {
        return emplace_back<scissor_command>();
    }

    viewport_command& command_buffer::viewport()
    {
        return emplace_back<viewport_command>();
    }

    fill_index_buffer_command& command_buffer::fill_index_buffer()
    {
        return emplace_back<fill_index_buffer_command>();
    }

    fill_vertex_buffer_command& command_buffer::fill_vertex_buffer()
    {
        return emplace_back<fill_vertex_buffer_command>();
    }

    generate_mipmaps_command& command_buffer::generate_mipmaps()
    {
        return emplace_back<generate_mipmaps_command>();
    }

    set_material_parameters_command& command_buffer::set_material_parameters()
    {
        return emplace_back<set_material_parameters_command>();
    }
} // namespace moka
