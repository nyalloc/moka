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

#include "graphics/api/graphics_api.hpp"
#include <application/signal.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>
namespace moka
{
    /**
     * \brief Rendering context handle
     */
    struct context_handle final
    {
        uint16_t id;
    };

    /**
     * \brief Window construction settings. Pass to the window object on constructions to set the initial state of the window.
     */
    struct window_settings final
    {
        std::string name = "moka";
        glm::ivec2 resolution = {1280, 720};
        glm::ivec2 position = {200, 200};
        bool fullscreen = false;
    };

    /**
     * \brief Windowing class. Abstracts the native windowing API. Exposes a high-level interface to interact with the window.
     */
    class window final
    {
    public:
        /**
         * \brief Exit signal. This signal notifies all slots when the user closes this window.
         */
        signal<> exit;

        window(const window& window) = delete;
        window(window&& window) = delete;
        window& operator=(const window& window) = delete;
        window& operator=(window&& window) = delete;

        /**
         * \brief Create a new window object.
         * \param settings The initial settings of the window under construction.
         */
        explicit window(const window_settings& settings);

        ~window();

        /**
         * \brief Swap this window's render buffer.
         */
        void swap_buffer() const;

        /**
         * \brief Update this window's dimensions.
         * \param width The new width of the window.
         * \param height The new height of the window.
         */
        void set_size(int width, int height);

        /**
         * \brief Create a new rendering context for this
         * \return
         */
        context_handle make_context() const;

        /**
         * \brief Set the current context to use with rendering to this window.
         * \param handle The context you wish to use with rendering to this window.
         */
        void set_current_context(context_handle handle);

        /**
         * \brief Get the aspect ratio of this window's size.
         * \return The width of the window divided by the height of the window.
         */
        float aspect() const;

        /**
         * \brief Get the size of the window.
         * \return A glm::ivec2 where x = width and y = height.
         */
        glm::ivec2 get_size() const;

        /**
         * \brief Get the drawable size of the window.
         * \return A glm::ivec2 where x == width and y == height.
         */
        glm::ivec2 get_drawable_size() const;

        /**
         * \brief Get the viewport of this window.
         * \return A rectangle defining the viewport of the drawable size. x == y == 0
         */
        rectangle get_viewport() const;

    private:
        class impl;
        std::unique_ptr<impl> impl_;
    };
} // namespace moka
