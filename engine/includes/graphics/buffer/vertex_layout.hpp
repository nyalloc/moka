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

#include <graphics/buffer/vertex_attribute.hpp>
#include <graphics/buffer/vertex_layout_builder.hpp>
#include <initializer_list>
#include <vector>

namespace moka
{
    /**
     * \brief Describes the layout of a single vertex in a vertex buffer.
     */
    class vertex_layout final
    {
    public:
        using builder = vertex_layout_builder;
        using iterator = std::vector<vertex_attribute>::iterator;
        using const_iterator = std::vector<vertex_attribute>::const_iterator;

        vertex_layout() = default;

        /**
         * \brief Create a vertex_layout object.
         * \param layout A initializer_list of vertex_attribute objects.
         */
        vertex_layout(std::initializer_list<vertex_attribute> layout);

        /**
         * \brief Create a vertex_layout object.
         * \param layout A vector of vertex_attribute objects.
         */
        explicit vertex_layout(std::vector<vertex_attribute>&& layout);

        iterator begin() noexcept;

        iterator end() noexcept;

        const_iterator begin() const noexcept;

        const_iterator end() const noexcept;

        /**
         * \brief Get the size of a single vertex in this vertex layout
         * \return The total size of a single vertex in this layout
         */
        size_t total_size() const noexcept;

    private:
        size_t total_size_ = 0;
        std::vector<vertex_attribute> layout_;
    };
} // namespace moka
