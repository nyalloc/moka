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

#include <cstdint>
#include <limits>
#include <vector>

namespace moka
{
    class graphics_device;

    /**
     * \brief The format of the framebuffer attachment.
     */
    enum class frame_format
    {
        depth_component24,
    };

    /**
     * \brief The type of framebuffer attachment.
     */
    enum class frame_attachment
    {
        color,        /**< A color framebuffer attachment*/
        depth,        /**< A depth framebuffer attachment*/
        stencil,      /**< A stencil framebuffer attachment*/
        depth_stencil /**< A depth stencil framebuffer attachment*/
    };

    /**
     * \brief A handle to a frame buffer object on the device.
     */
    struct frame_buffer_handle final
    {
        uint16_t id = std::numeric_limits<uint16_t>::max();

        bool operator==(const frame_buffer_handle& rhs) const;

        bool operator!=(const frame_buffer_handle& rhs) const;

        bool operator>(const frame_buffer_handle& rhs) const;

        bool operator<(const frame_buffer_handle& rhs) const;

        bool operator>=(const frame_buffer_handle& rhs) const;

        bool operator<=(const frame_buffer_handle& rhs) const;
    };

    /**
     * \brief Data describing a framebuffer attachment.
     */
    struct render_texture_data final
    {
        frame_attachment attachment; /**< The type of this attachment.*/
        frame_format format;         /**< The format of this attachment.*/
        int width;                   /**< The width of this attachment.*/
        int height;                  /**< The height of this attachment.*/
    };

    /**
     * \brief A builder class to help you build a frame buffer.
     */
    struct frame_buffer_builder final
    {
        /**
         * \brief Build a frame buffer builder
         * \param device The graphics_device object that this frame buffer will be uploaded to
         */
        explicit frame_buffer_builder(graphics_device& device);

        std::vector<render_texture_data> render_attachments; /**< A contiguous buffer of frame buffer attachments.*/

        /**
         * \brief Add a depth attachment to the framebuffer.
         * \param format The format of the depth attachment.
         * \param width The width of the depth attachment.
         * \param height The height of the depth attachment.
         * \return A reference to this frame_buffer_builder object to enable method chaining.
         */
        frame_buffer_builder& add_depth_attachment(frame_format format, int width, int height);

        /**
         * \brief Add a color attachment to the framebuffer.
         * \param format The format of the color attachment.
         * \param width The width of the color attachment.
         * \param height The height of the color attachment.
         * \return A reference to this frame_buffer_builder object to enable method chaining.
         */
        frame_buffer_builder& add_color_attachment(frame_format format, int width, int height);

        /**
         * \brief Add a stencil attachment to the framebuffer.
         * \param format The format of the stencil attachment.
         * \param width The width of the stencil attachment.
         * \param height The height of the stencil attachment.
         * \return A reference to this frame_buffer_builder object to enable method chaining.
         */
        frame_buffer_builder& add_stencil_attachment(frame_format format, int width, int height);

        /**
         * \brief Add a depth stencil attachment to the framebuffer.
         * \param format The format of the depth stencil attachment.
         * \param width The width of the depth stencil attachment.
         * \param height The height of the depth stencil attachment.
         * \return A reference to this frame_buffer_builder object to enable method chaining.
         */
        frame_buffer_builder& add_depth_stencil_attachment(frame_format format, int width, int height);

        /**
         * \brief Build the final frame buffer, upload it to the device, and
         * return the handle. \return The handle to the new frame buffer.
         */
        frame_buffer_handle build();

    private:
        graphics_device& device_;
    };
} // namespace moka
