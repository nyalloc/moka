#pragma once
#include "base_camera.hpp"
#include <memory>

namespace moka
{
    // camera decorator. subclass this to add extra functionality to a camera
    class camera_decorator : public base_camera
    {
    protected:
        std::unique_ptr<base_camera> camera_;

    public:
        camera_decorator(std::unique_ptr<base_camera>&& camera);

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
#
        void set_perspective(float radians, float aspect) override;
    };
} // namespace moka
