#include "imgui/imgui.h"
#include <graphics/camera/camera_mouse_controller.hpp>

namespace moka
{
    camera_mouse_controller::camera_mouse_controller(std::unique_ptr<base_camera>&& camera, mouse& mouse)
        : camera_decorator(std::move(camera)), mouse_(mouse)
    {
    }

    glm::mat4 camera_mouse_controller::get_view() const
    {
        return camera_->get_view();
    }

    void camera_mouse_controller::update(const float delta_time)
    {
        auto mouse_state = mouse_.get_state();
        const auto motion = mouse_state.get_motion();
        const auto scroll = mouse_state.get_scroll();

        const auto& io = ImGui::GetIO();

        if (auto_scroll_)
        {
            rotate_x_ -= delta_time * 0.2f;

            if ((mouse_state.is_button_down(mouse_button::left) ||
                 mouse_state.is_button_down(mouse_button::right)) &&
                !io.WantCaptureMouse)
            {
                auto_scroll_ = false;
            }
        }
        else
        {
            if (mouse_state.is_button_down(mouse_button::left) && !io.WantCaptureMouse)
            {
                rotate_x_ -= motion.x * delta_time;
                rotate_y_ -= motion.y * delta_time;
            }
            else if (mouse_state.is_button_down(mouse_button::right) && !io.WantCaptureMouse)
            {
                translate_z_ += motion.y * delta_time;
            }

            if (scroll.y != 0 && !io.WantCaptureMouse)
            {
                translate_z_ -= scroll.y * delta_time;
            }
        }

        rotate_y_ = glm::clamp(rotate_y_, glm::radians(-89.0f), glm::radians(89.0f));
        translate_z_ = glm::clamp(translate_z_, 0.3f, 100.0f);

        current_rotate_x_ = glm::mix(current_rotate_x_, rotate_x_, delta_time * 5);
        current_rotate_y_ = glm::mix(current_rotate_y_, rotate_y_, delta_time * 5);
        current_translate_z_ =
            glm::mix(current_translate_z_, translate_z_, delta_time * 5);

        auto trans = get_transform();

        trans.set_position(glm::vec3{0, 0, current_translate_z_});
        trans.rotate_around(transform::world_origin(), transform::world_right(), current_rotate_y_);
        trans.rotate_around(transform::world_origin(), transform::world_up(), current_rotate_x_);

        trans.look_at(transform::world_origin());

        set_transform(trans);
    }
} // namespace moka
