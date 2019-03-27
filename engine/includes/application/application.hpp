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

#include <application/app_settings.hpp>
#include <application/logger.hpp>
#include <application/timer.hpp>
#include <application/window.hpp>
#include <filesystem>
#include <graphics/device/graphics_device.hpp>
#include <input/keyboard.hpp>
#include <input/mouse.hpp>

namespace moka
{
    using game_time = float;

    /**
     * \brief The basic skeleton of a moka application. Contains all windowing, input-handling + rendering logic. Child classes define the fleshed out application-specific functionality.
     */
    class application
    {
        bool running_ = true;
        void poll_events();

    protected:
        logger log_;  /**< A basic logger. */
        timer timer_; /**< A stopwatch-style timer with std::chrono support. */
        window window_; /**< Windowing class. Abstracts the native windowing API. */
        mouse mouse_; /**< Contains the mouse state as last reported by the native input API */
        keyboard keyboard_; /**< Contains the keyboard state as last reported by the native input API */
        graphics_device graphics_; /**< An abstraction of the native rendering API. Resources can be created + destroyed, and drawing commands can be submitted through command lists. */

    public:
        /**
         * \brief Construct a new application object.
         * \param settings The application settings to be used when setting up the input, windowing and rendering systems.
         */
        explicit application(const app_settings& settings);

        application(const application& rhs) = delete;

        application(application&& rhs) = delete;

        application& operator=(const application& rhs) = delete;

        application& operator=(application&& rhs) = delete;

        virtual ~application();

        /**
         * \brief Get the number of seconds since the application was started.
         * \return The number of seconds since the application started.
         */
        float seconds_elapsed() const;

        /**
         * \brief A draw method that can be overriden by child classes to add application-specific rendering functionality.
         * This function is called by the main game loop at a variable time rate to decouple the simulation from the rendering framerate.
         * \param delta_time The time in seconds since the last invocation of this function.
         */
        virtual void draw(game_time delta_time) = 0;

        /**
         * \brief An update method that can be overriden by child classes to add application-specific functionality.
         * This function is called by the main game loop at a fixed rate of 60 times a second.
         * \param delta_time The time in seconds since the last invocation of this function.
         */
        virtual void update(game_time delta_time) = 0;

        /**
         * \brief Starts the game loop
         * \return 0 if the application exits successfully, 1 if the application exits with an error.
         */
        int run();

        /**
         * \brief Get the absolute path to the root resources folder for this application.
         * \return The absolute path to the root resources folder for this application.
         */
        virtual std::filesystem::path data_path() = 0;
    };
} // namespace moka
