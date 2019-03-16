#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <graphics/camera/basic_camera.hpp>

namespace moka
{
    basic_camera::basic_camera(const transform& transform, const glm::mat4& projection)
        : transform_(transform), projection_(projection)
    {
    }

    void basic_camera::update(const float delta_time)
    {
    }

    glm::mat4 basic_camera::get_view() const
    {
        return glm::lookAt(
            transform_.get_position(), transform_.get_position() + transform_.front(), transform_.up());
    }

    const glm::quat& basic_camera::get_rotation() const
    {
        return transform_.get_rotation();
    }

    void basic_camera::set_rotation(const glm::quat& rotation)
    {
        return transform_.set_rotation(rotation);
    }

    const glm::vec3& basic_camera::get_position() const
    {
        return transform_.get_position();
    }

    void basic_camera::set_position(const glm::vec3& position)
    {
        return transform_.set_position(position);
    }

    const glm::mat4& basic_camera::get_projection() const
    {
        return projection_;
    }

    void basic_camera::set_projection(const glm::mat4& projection)
    {
        projection_ = projection;
    }

    void basic_camera::set_transform(const transform& transform)
    {
        transform_ = transform;
    }

    const transform& basic_camera::get_transform() const
    {
        return transform_;
    }

    void basic_camera::set_perspective(const float radians, const float aspect)
    {
        projection_ = glm::perspective(radians, aspect, 0.1f, 100.0f);
    }
} // namespace moka
