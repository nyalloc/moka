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

#include <graphics/device/graphics_device.hpp>
#include <graphics/material/material.hpp>
#include <graphics/transform.hpp>

namespace moka
{
    /**
     * \brief A basic primitive. A wrapper around a vertex buffer, an index buffer and a material.
     */
    class primitive
    {
        vertex_buffer_handle vertex_buffer_;
        uint32_t vertex_count_ = 0;

        index_buffer_handle index_buffer_;
        index_type index_type_;
        uint32_t index_count_ = 0;
        uint32_t index_buffer_offset_ = 0;

        primitive_type type_ = primitive_type::triangles;

        material_handle material_;

    public:
        material_handle get_material() const;

        primitive(
            vertex_buffer_handle vertex_buffer,
            uint32_t vertex_count,
            index_buffer_handle index_buffer,
            index_type index_type,
            uint32_t index_count,
            uint32_t index_buffer_offset,
            material_handle material);

        primitive(vertex_buffer_handle vertex_buffer, uint32_t vertex_count, material_handle material);

        void draw(command_buffer& list) const;
    };

    /**
     * \brief A mesh class, a wrapper around multiple primitives and a transform.
     */
    class mesh
    {
        std::vector<primitive> primitives_;

        transform transform_;

    public:
        using iterator = std::vector<primitive>::iterator;
        using const_iterator = std::vector<primitive>::const_iterator;

        transform& get_transform();

        iterator begin();

        const_iterator begin() const;

        iterator end();

        const_iterator end() const;

        explicit mesh(std::vector<primitive>&& primitives, transform&& transform = {});

        explicit mesh(const primitive& prim, transform&& transform = {});
    };

    /**
     * \brief A model class, a wrapper around multiple meshes and a transform.
     */
    class model
    {
        std::vector<mesh> meshes_;
        transform transform_;

    public:
        using iterator = std::vector<mesh>::iterator;
        using const_iterator = std::vector<mesh>::const_iterator;

        iterator begin();

        const_iterator begin() const;

        iterator end();

        const_iterator end() const;

        model() = default;

        explicit model(const mesh& meshes, transform&& transform = {});

        explicit model(std::vector<mesh>&& meshes, transform&& transform = {});

        std::vector<mesh>& get_meshes();
    };
} // namespace moka
