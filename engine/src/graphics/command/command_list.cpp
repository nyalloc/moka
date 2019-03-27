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
#include <algorithm>
#include <graphics/command/command_list.hpp>

namespace moka
{
    bool command_list::key_comparator::operator()(const command_buffer& left, const command_buffer& right) const
    {
        return left.get_key() < right.get_key();
    }

    command_list::command_list() = default;

    command_list::~command_list() = default;

    command_list::command_list(command_list&& command_list) noexcept
        : command_packets_(std::move(command_list.command_packets_))
    {
    }

    command_list& command_list::operator=(command_list&& command_list) noexcept
    {
        command_packets_ = std::move(command_list.command_packets_);
        return *this;
    }

    void command_list::destroy()
    {
        command_packets_.clear();
    }

    void command_list::accept(graphics_visitor& device)
    {
        for (auto& command_packet : command_packets_)
        {
            command_packet.accept(device);
        }
    }

    void command_list::sort()
    {
        std::sort(command_packets_.begin(), command_packets_.end(), key_comparator{});
        is_sorted_ = true;
    }

    bool command_list::is_sorted() const
    {
        return is_sorted_;
    }

    bool command_list::is_empty() const
    {
        return command_packets_.empty();
    }

    command_list::iterator command_list::begin()
    {
        return command_packets_.begin();
    }

    command_list::const_iterator command_list::begin() const
    {
        return command_packets_.begin();
    }

    command_list::iterator command_list::end()
    {
        return command_packets_.end();
    }

    command_list::const_iterator command_list::end() const
    {
        return command_packets_.end();
    }

    command_buffer& command_list::make_command_buffer(const sort_key key)
    {
        static auto num_calls = 0;
        num_calls++;
        is_sorted_ = false;

        current_key_ = key;
        command_packets_.emplace_back(key);

        return command_packets_.back();
    }

    command_buffer& command_list::make_command_buffer()
    {
        return make_command_buffer(current_key_ + 1);
    }

    set_material_parameters_command& command_list::set_material_parameters()
    {
        return make_command_buffer().set_material_parameters();
    }

    set_material_parameters_command& command_list::set_material_parameters(sort_key key)
    {
        return make_command_buffer(key).set_material_parameters();
    }

    clear_command& command_list::clear()
    {
        return make_command_buffer().clear();
    }

    clear_command& command_list::clear(const sort_key key)
    {
        return make_command_buffer(key).clear();
    }

    frame_buffer_command& command_list::frame_buffer()
    {
        return make_command_buffer().frame_buffer();
    }

    frame_buffer_command& command_list::frame_buffer(const sort_key key)
    {
        return make_command_buffer(key).frame_buffer();
    }

    frame_buffer_texture_command& command_list::frame_buffer_texture()
    {
        return make_command_buffer().frame_buffer_texture();
    }

    frame_buffer_texture_command& command_list::frame_buffer_texture(const sort_key key)
    {
        return make_command_buffer(key).frame_buffer_texture();
    }

    draw_command& command_list::draw()
    {
        return make_command_buffer().draw();
    }

    draw_command& command_list::draw(const sort_key key)
    {
        return make_command_buffer(key).draw();
    }

    scissor_command& command_list::scissor()
    {
        return make_command_buffer().scissor();
    }

    scissor_command& command_list::scissor(const sort_key key)
    {
        return make_command_buffer(key).scissor();
    }

    viewport_command& command_list::viewport()
    {
        return make_command_buffer().viewport();
    }

    viewport_command& command_list::viewport(const sort_key key)
    {
        return make_command_buffer(key).viewport();
    }

    fill_index_buffer_command& command_list::fill_index_buffer()
    {
        return make_command_buffer().fill_index_buffer();
    }

    fill_index_buffer_command& command_list::fill_index_buffer(const sort_key key)
    {
        return make_command_buffer(key).fill_index_buffer();
    }

    fill_vertex_buffer_command& command_list::fill_vertex_buffer()
    {
        return make_command_buffer().fill_vertex_buffer();
    }

    fill_vertex_buffer_command& command_list::fill_vertex_buffer(const sort_key key)
    {
        return make_command_buffer(key).fill_vertex_buffer();
    }

    generate_mipmaps_command& command_list::generate_mipmaps()
    {
        return make_command_buffer().generate_mipmaps();
    }

    generate_mipmaps_command& command_list::generate_mipmaps(sort_key key)
    {
        return make_command_buffer(key).generate_mipmaps();
    }
} // namespace moka
