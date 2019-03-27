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

#include <glm/detail/type_vec4.hpp>
#include <graphics/command/graphics_command.hpp>

namespace moka
{
    /**
     * \brief Clear the current frame buffer.
     */
    class clear_command final : public graphics_command
    {
    public:
        glm::vec4 color; /**< The color that should be used to clear the buffer. */
        bool clear_depth = false; /**< Should we clear the depth information? */
        bool clear_color = false; /**< Should we clear the color buffer? */

        virtual ~clear_command();

        /**
         * \brief Accept a graphics_visitor object. Invoke this command using
         * the graphics_visitor. \param visitor A graphics_visitor object.
         */
        void accept(graphics_visitor& visitor) override;

        /**
         * \brief Set the color of the clear_command.
         * \param color The color that should be used.
         * \return A reference to this clear_command object to enable method chaining.
         */
        clear_command& set_color(const glm::vec4& color);

        /**
         * \brief Set the color of the clear_command.
         * \param r The r component of the color.
         * \param g The g component of the color.
         * \param b The b component of the color.
         * \param a The a component of the color.
         * \return A reference to this clear_command object to enable method chaining.
         */
        clear_command& set_color(float r, float g, float b, float a);

        /**
         * \brief Should this clear_command clear the color buffer?
         * \param val True if the color should be cleared, otherwise false.
         * \return A reference to this clear_command object to enable method chaining.
         */
        clear_command& set_clear_color(bool val);

        /**
         * \brief Should this clear_command clear the depth buffer?
         * \param val True if the depth should be cleared, otherwise false.
         * \return A reference to this clear_command object to enable method chaining.
         */
        clear_command& set_clear_depth(bool val);
    };
} // namespace moka
