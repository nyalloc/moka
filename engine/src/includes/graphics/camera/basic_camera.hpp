#pragma once

#include <glm/glm.hpp>
#include <graphics/transform.hpp>

namespace moka
{
    // a very basic camera - contains only the necessary data members to get things drawing on your screen.
    // can be used as the base concrete type of a decorator-based camera
    class basic_camera
    {
    protected:
        transform transform_;
        glm::mat4 projection_;

    public:
        basic_camera() = default;

        basic_camera(const transform& transform, const glm::mat4& projection);

        virtual void update(float delta_time);

        virtual glm::mat4 get_view() const;

        virtual const glm::quat& get_rotation() const;

        virtual void set_rotation(const glm::quat& rotation);

        virtual const glm::vec3& get_position() const;

        virtual void set_position(const glm::vec3& position);

        virtual const glm::mat4& get_projection() const;

        virtual void set_projection(const glm::mat4& projection);

        virtual void set_transform(const transform& transform);

        virtual const transform& get_transform() const;

        virtual void set_perspective(float radians, float aspect);
    };
} // namespace moka
