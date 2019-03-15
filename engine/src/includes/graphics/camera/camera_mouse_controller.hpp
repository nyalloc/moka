#pragma once

#include <graphics/camera/basic_camera.hpp>
#include <input/mouse.hpp>

namespace moka
{
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
        ~camera_mouse_controller() = default;
        camera_mouse_controller(const camera_mouse_controller& camera) = delete;
        camera_mouse_controller(camera_mouse_controller&& camera) noexcept = delete;
        camera_mouse_controller& operator=(const camera_mouse_controller& camera) = delete;
        camera_mouse_controller& operator=(camera_mouse_controller&& camera) noexcept = delete;

        camera_mouse_controller(mouse& mouse, const glm::mat4& perspective);

        void set_auto_rotate(bool rotate);

        void update(float delta_time) override;
    };
} // namespace moka
