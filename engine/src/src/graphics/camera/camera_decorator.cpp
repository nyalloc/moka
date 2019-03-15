#include <graphics/camera/camera_decorator.hpp>

namespace moka
{
    camera_decorator::camera_decorator(std::unique_ptr<base_camera>&& camera)
        : camera_(std::move(camera))
    {
    }

    void camera_decorator::update(const float delta_time)
    {
    }

    glm::mat4 camera_decorator::get_view() const
    {
        return camera_->get_view();
    }

    const glm::quat& camera_decorator::get_rotation() const
    {
        return camera_->get_rotation();
    }

    void camera_decorator::set_rotation(const glm::quat& rotation)
    {
        camera_->set_rotation(rotation);
    }

    const glm::vec3& camera_decorator::get_position() const
    {
        return camera_->get_position();
    }

    void camera_decorator::set_position(const glm::vec3& position)
    {
        camera_->set_position(position);
    }

    const glm::mat4& camera_decorator::get_projection() const
    {
        return camera_->get_projection();
    }

    void camera_decorator::set_projection(const glm::mat4& projection)
    {
        camera_->set_projection(projection);
    }

    void camera_decorator::set_transform(const transform& transform)
    {
        camera_->set_transform(transform);
    }

    const transform& camera_decorator::get_transform() const
    {
        return camera_->get_transform();
    }

    void camera_decorator::set_perspective(const float radians, const float aspect)
    {
        return camera_->set_perspective(radians, aspect);
    }
} // namespace moka
