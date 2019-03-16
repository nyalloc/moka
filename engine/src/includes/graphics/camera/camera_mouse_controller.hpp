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
