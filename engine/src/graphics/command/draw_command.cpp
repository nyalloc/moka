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
#include <graphics/command/draw_command.hpp>

namespace moka
{
    draw_command::draw_command() = default;

    draw_command::~draw_command() = default;

    draw_command::draw_command(const draw_command& rhs)
        : material(rhs.material),
          vertex_buffer(rhs.vertex_buffer),
          vertex_count(rhs.vertex_count),
          first_vertex(rhs.first_vertex),
          index_buffer(rhs.index_buffer),
          index_count(rhs.index_count),
          idx_type(rhs.idx_type),
          index_buffer_offset(rhs.index_buffer_offset),
          prim_type(rhs.prim_type)
    {
    }

    draw_command& draw_command::operator=(const draw_command& rhs)
    {
        material = rhs.material;
        vertex_buffer = rhs.vertex_buffer;
        vertex_count = rhs.vertex_count;
        first_vertex = rhs.first_vertex;
        index_buffer = rhs.index_buffer;
        index_count = rhs.index_count;
        idx_type = rhs.idx_type;
        index_buffer_offset = rhs.index_buffer_offset;
        prim_type = rhs.prim_type;
        return *this;
    }

    draw_command::draw_command(draw_command&& rhs) noexcept
        : material(std::move(rhs.material)),
          vertex_buffer(rhs.vertex_buffer),
          vertex_count(rhs.vertex_count),
          first_vertex(rhs.first_vertex),
          index_buffer(rhs.index_buffer),
          index_count(rhs.index_count),
          idx_type(rhs.idx_type),
          index_buffer_offset(rhs.index_buffer_offset),
          prim_type(rhs.prim_type)
    {
    }

    draw_command& draw_command::operator=(draw_command&& rhs) noexcept
    {
        material = std::move(rhs.material);
        vertex_buffer = rhs.vertex_buffer;
        vertex_count = rhs.vertex_count;
        first_vertex = rhs.first_vertex;
        index_buffer = rhs.index_buffer;
        index_count = rhs.index_count;
        idx_type = rhs.idx_type;
        index_buffer_offset = rhs.index_buffer_offset;
        prim_type = rhs.prim_type;
        return *this;
    }

    void draw_command::accept(graphics_visitor& visitor)
    {
        visitor.visit(*this);
    }

    draw_command& draw_command::set_index_buffer_offset(uint32_t offset)
    {
        this->index_buffer_offset = offset;
        return *this;
    }

    draw_command& draw_command::set_index_type(index_type type)
    {
        this->idx_type = type;
        return *this;
    }

    draw_command& draw_command::set_primitive_type(primitive_type prim_type)
    {
        this->prim_type = prim_type;
        return *this;
    }

    draw_command& draw_command::set_index_count(uint32_t count)
    {
        this->index_count = count;
        return *this;
    }

    draw_command& draw_command::set_first_vertex(const uint32_t first)
    {
        this->first_vertex = first;
        return *this;
    }

    draw_command& draw_command::set_vertex_count(const uint32_t count)
    {
        this->vertex_count = count;
        return *this;
    }

    draw_command& draw_command::set_material(const material_handle material)
    {
        this->material = material;
        return *this;
    }

    draw_command& draw_command::set_vertex_buffer(const moka::vertex_buffer_handle vertex_buffer)
    {
        this->vertex_buffer = vertex_buffer;
        return *this;
    }

    draw_command& draw_command::set_index_buffer(moka::index_buffer_handle index_buffer)
    {
        this->index_buffer = index_buffer;
        return *this;
    }
} // namespace moka
