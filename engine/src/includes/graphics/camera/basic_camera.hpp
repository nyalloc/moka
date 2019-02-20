#pragma once

#include <graphics/camera/base_camera.hpp>

namespace moka
{
    // a very basic camera - contains only the necessary data members to get things drawing on your screen.
    // can be used as the base concrete type of a decorator-based camera
    class basic_camera : public base_camera
    {
        transform transform_;
        glm::mat4 projection_;

    public:
        basic_camera() = default;

        basic_camera(const transform& transform, const glm::mat4& projection);

        void update(float delta_time) override;

        glm::mat4 get_view() const override;

        const glm::quat& get_rotation() const override;

        void set_rotation(const glm::quat& rotation) override;

        const glm::vec3& get_position() const override;

        void set_position(const glm::vec3& position) override;

        const glm::mat4& get_projection() const override;

        void set_projection(const glm::mat4& projection) override;

        void set_transform(const transform& transform) override;

        const transform& get_transform() const override;

        void set_perspective(float radians, float aspect) override;
    };
} // namespace moka
