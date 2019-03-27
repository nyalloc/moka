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
#include <graphics/texture_handle.hpp>

namespace moka
{
    /**
     * \brief Set the texture you want to attach to this frame buffer.
     */
    class frame_buffer_texture_command final : public graphics_command
    {
    public:
        texture_handle texture; /**< Specifies the texture object whose image is to be attached. */
        frame_attachment attachment; /**< Specifies the attachment to which the texture should be attached. */
        image_target target; /**< Specifies the texture target.  */
        int level; /**< Specifies the mipmap level of the texture image to be attached, which must be 0.*/

        virtual ~frame_buffer_texture_command();

        /**
         * \brief Accept a graphics_visitor object. Invoke this command using
         * the graphics_visitor. \param visitor A graphics_visitor object.
         */
        void accept(graphics_visitor& visitor) override;

        /**
         * \brief Set the texture whose image is to be attached.
         * \param texture The texture whose image is to be attached.
         * \return A reference to this frame_buffer_texture_command object to enable method chaining.
         */
        frame_buffer_texture_command& set_texture(texture_handle texture);

        /**
         * \brief Set the attachment to which the texture should be attached.
         * \param attachment The attachment to which the texture should be attached.
         * \return A reference to this frame_buffer_texture_command object to enable method chaining.
         */
        frame_buffer_texture_command& set_attachment(frame_attachment attachment);

        /**
         * \brief Set the texture target.
         * \param target The texture target.
         * \return A reference to this frame_buffer_texture_command object to enable method chaining.
         */
        frame_buffer_texture_command& set_target(image_target target);

        /**
         * \brief Set the mipmap level of the texture image to be attached.
         * \param level The mipmap level of the texture image to be attached.
         * \return A reference to this frame_buffer_texture_command object to enable method chaining.
         */
        frame_buffer_texture_command& set_mip_level(int level);
    };
} // namespace moka
