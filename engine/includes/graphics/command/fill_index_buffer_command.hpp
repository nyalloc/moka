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

#include <graphics/buffer/index_buffer_handle.hpp>
#include <graphics/command/graphics_command.hpp>

namespace moka
{
    /**
     * \brief Fill an index buffer.
     */
    class fill_index_buffer_command final : public graphics_command
    {
    public:
        index_buffer_handle handle; /**< The index buffer you want to fill. */
        const void* data;           /**< The host buffer of index data. */
        size_t size;                /**< The size of the host buffer. */

        virtual ~fill_index_buffer_command();

        /**
         * \brief Accept a graphics_visitor object. Invoke this command using
         * the graphics_visitor. \param visitor A graphics_visitor object.
         */
        void accept(graphics_visitor& visitor) override;

        /**
         * \brief Set the index buffer that you want to fill.
         * \param handle The index buffer you want to fill.
         * \param data The host buffer data you want to fill the index buffer data.
         * \param size The size of the host buffer data.
         * \return A reference to this fill_index_buffer_command object to enable method chaining.
         */
        fill_index_buffer_command& set_buffer(index_buffer_handle handle, const void* data, size_t size);
    };
} // namespace moka
