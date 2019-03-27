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

#include <array>
#include <glm/vec2.hpp>

namespace moka
{
    enum class mouse_button : uint8_t
    {
        left = 0,
        right,
        middle
    };

    /**
     * \brief This contains a snapshot of the mouse state.
     */
    class mouse_state
    {
        friend class application;

        glm::ivec2 position_;
        glm::ivec2 motion_;
        std::array<bool, 3> buttons_ = {};
        glm::ivec2 scroll_;

    public:
        /**
         * \brief Get the current mouse position.
         * \return The current mouse position.
         */
        const glm::ivec2& get_position() const;

        /**
         * \brief Get the mouse motion since the last update.
         * \return The mouse motion since the last update.
         */
        const glm::ivec2& get_motion() const;

        /**
         * \brief Check if a button is down.
         * \param button The button you want to check.
         * \return True if the button is down, otherwise false.
         */
        bool is_button_down(mouse_button button) const;

        /**
         * \brief Check if a button is up.
         * \param button The button you want to check.
         * \return True if the button is up, otherwise false.
         */
        bool is_button_up(mouse_button button) const;

        /**
         * \brief Get the scroll wheel state.
         * \return The scroll wheel state.
         */
        glm::ivec2 get_scroll() const;
    };

    /**
     * \brief The mouse object. Updated in the main event loop so that it contains an up-to-date snapshot of the mouse state.
     */
    class mouse
    {
        friend class application;

        mouse_state state_;

    public:
        mouse() = default;
        mouse(const mouse& rhs) = delete;
        mouse(mouse&& rhs) = delete;
        mouse& operator=(const mouse& rhs) = delete;
        mouse& operator=(mouse&& rhs) = delete;
        ~mouse() = default;

        /**
         * \brief Get the latest mouse state.
         * \return The latest mouse state.
         */
        const mouse_state& get_state() const;
    };
} // namespace moka
