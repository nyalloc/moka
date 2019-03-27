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
#include <vector>

namespace moka
{
    class vertex_layout;

    /**
     * \brief A builder class to help you build a vertex.
     */
    struct vertex_layout_builder final
    {
    private:
        std::vector<vertex_attribute> attr_;

    public:
        /**
         * \brief Add a vertex attribute to this layout.
         * \param index The index of the new vertex_attribute object.
         * \param type The type of the new vertex_attribute object.
         * \param size The size of the new vertex_attribute object.
         * \param normalized Should the new vertex_attribute be normalized?
         * \param stride The stride of the new vertex_attribute object.
         * \param offset The offset of the new vertex_attribute object.
         * \return A reference to this vertex_layout_builder object to enable method chaining.
         */
        vertex_layout_builder& add_attribute(
            size_t index, size_t size, attribute_type type, bool normalized, size_t stride, size_t offset);

        /**
         * \brief Build the final vertex layout, upload it to the device, and
         * return the handle. \return The handle to the new vertex layout.
         */
        vertex_layout build();
    };

} // namespace moka
