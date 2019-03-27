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

#include <graphics/buffer/frame_buffer_handle.hpp>
#include <graphics/command/graphics_command.hpp>

namespace moka
{
    /**
     * \brief Set the current frame buffer. This will modify all subsequent draw commands in this list.
     */
    class frame_buffer_command final : public graphics_command
    {
    public:
        frame_buffer_handle buffer; /**< The current frame buffer you want to use. */

        frame_buffer_command();

        ~frame_buffer_command();

        frame_buffer_command(const frame_buffer_command& rhs);

        frame_buffer_command& operator=(const frame_buffer_command& rhs);

        frame_buffer_command(frame_buffer_command&& rhs) noexcept;

        frame_buffer_command& operator=(frame_buffer_command&& rhs) noexcept;

        /**
         * \brief Accept a graphics_visitor object. Invoke this command using
         * the graphics_visitor. \param visitor A graphics_visitor object.
         */
        void accept(graphics_visitor& visitor) override;

        /**
         * \brief Set the current frame buffer.
         * \param buffer Set the current frame buffer.
         * \return A reference to this frame_buffer_command object to enable method chaining.
         */
        frame_buffer_command& set_frame_buffer(frame_buffer_handle buffer);
    };
} // namespace moka
