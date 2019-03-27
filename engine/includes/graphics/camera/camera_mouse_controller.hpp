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

#include <graphics/camera/basic_camera.hpp>
#include <input/mouse.hpp>

namespace moka
{
    /**
     * \brief A mouse-controlled camera.
     */
    class camera_mouse_controller final : public basic_camera
    {
        mouse& mouse_;

        float current_translate_z_ = 10.0f;
        float current_rotate_x_ = -2 * glm::pi<float>();
        float current_rotate_y_ = -0.0f;

        float translate_z_ = 10.0f;
        float rotate_x_ = -2 * glm::pi<float>();
        float rotate_y_ = -0.0f;

        bool auto_rotate_ = false;

    public:
        virtual ~camera_mouse_controller() = default;

        camera_mouse_controller(const camera_mouse_controller& camera) = delete;
        camera_mouse_controller(camera_mouse_controller&& camera) noexcept = delete;
        camera_mouse_controller& operator=(const camera_mouse_controller& camera) = delete;
        camera_mouse_controller& operator=(camera_mouse_controller&& camera) noexcept = delete;

        /**
         * \brief Create a new camera_mouse_controller object.
         * \param mouse The mouse to attach to the camera.
         * \param perspective The perspective of the camera.
         */
        camera_mouse_controller(mouse& mouse, const glm::mat4& perspective);

        /**
         * \brief Should this camera auto rotate around the target?
         * \param rotate True if the camera should auto-rotate, false if it should not.
         */
        void set_auto_rotate(bool rotate);

        /**
         * \brief Update the camera.
         * \param delta_time The current delta-time.
         */
        void update(float delta_time) override;
    };
} // namespace moka
