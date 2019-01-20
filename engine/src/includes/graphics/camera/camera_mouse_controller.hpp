#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <graphics/camera/camera_decorator.hpp>
#include <input/mouse.hpp>

namespace moka
{
    class camera_mouse_controller final : public camera_decorator
    {
        mouse& mouse_;

        float current_translate_z_ = 3.0f;
        float current_rotate_x_ = -glm::pi<float>();
        float current_rotate_y_ = 0.3f;

        float translate_z_ = 3.0f;
        float rotate_x_ = -glm::pi<float>();
        float rotate_y_ = 0.3f;

        bool auto_scroll_ = true;

    public:
        ~camera_mouse_controller() = default;
        camera_mouse_controller(const camera_mouse_controller& camera) = delete;
        camera_mouse_controller(camera_mouse_controller&& camera) noexcept = delete;
        camera_mouse_controller& operator=(const camera_mouse_controller& camera) = delete;
        camera_mouse_controller& operator=(camera_mouse_controller&& camera) noexcept = delete;

        camera_mouse_controller(std::unique_ptr<base_camera>&& camera, mouse& mouse);

        glm::mat4 get_view() const override;

        void update(float delta_time) override;
    };
} // namespace moka
