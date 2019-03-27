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
#include "imgui/imgui.h"
#include <graphics/camera/camera_mouse_controller.hpp>

namespace moka
{
    camera_mouse_controller::camera_mouse_controller(mouse& mouse, const glm::mat4& perspective)
        : basic_camera(transform(), perspective), mouse_(mouse)
    {
    }

    void camera_mouse_controller::set_auto_rotate(const bool rotate)
    {
        auto_rotate_ = rotate;
    }

    void camera_mouse_controller::update(const float delta_time)
    {
        auto mouse_state = mouse_.get_state();
        const auto motion = mouse_state.get_motion();
        const auto scroll = mouse_state.get_scroll();

        const auto& io = ImGui::GetIO();

        if (mouse_state.is_button_down(mouse_button::left) && !io.WantCaptureMouse)
        {
            rotate_x_ -= motion.x * delta_time;
            rotate_y_ -= motion.y * delta_time;
        }
        else
        {
            if (auto_rotate_)
            {
                rotate_x_ -= delta_time * 0.2f;
            }
        }

        if (mouse_state.is_button_down(mouse_button::right) && !io.WantCaptureMouse)
        {
            translate_z_ += motion.y * delta_time;
        }

        if (scroll.y != 0 && !io.WantCaptureMouse)
        {
            translate_z_ -= scroll.y * 5 * delta_time;
        }

        rotate_y_ = glm::clamp(rotate_y_, glm::radians(-89.0f), glm::radians(89.0f));
        translate_z_ = glm::clamp(translate_z_, 0.3f, 100.0f);

        current_rotate_x_ = glm::mix(current_rotate_x_, rotate_x_, delta_time * 5);
        current_rotate_y_ = glm::mix(current_rotate_y_, rotate_y_, delta_time * 5);
        current_translate_z_ =
            glm::mix(current_translate_z_, translate_z_, delta_time * 5);

        transform_.set_position(glm::vec3{0, 0, current_translate_z_});
        transform_.rotate_around(
            transform::world_origin(), transform::world_right(), current_rotate_y_);
        transform_.rotate_around(
            transform::world_origin(), transform::world_up(), current_rotate_x_);
        transform_.look_at(transform::world_origin());
    }
} // namespace moka
