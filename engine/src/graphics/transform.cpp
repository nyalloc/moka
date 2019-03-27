#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <graphics/transform.hpp>

namespace moka
{
    transform::transform(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation)
        : position_(position), scale_(scale), rotation_(glm::normalize(rotation))
    {
    }

    transform::transform()
        : position_{0.0f, 0.0f, 0.0f}, scale_{1.0f, 1.0f, 1.0f}, rotation_{1.0f, glm::vec3()}
    {
    }

    transform::transform(const transform& rhs) = default;
    transform& transform::operator=(const transform& rhs) = default;
    transform::transform(transform&& rhs) noexcept = default;
    transform& transform::operator=(transform&& rhs) noexcept = default;

    const glm::vec3& transform::get_position() const
    {
        return position_;
    }

    glm::vec3 transform::get_world_position() const
    {
        return position_ * rotation_ * scale_;
    }

    const glm::vec3& transform::get_scale() const
    {
        return scale_;
    }

    const glm::quat& transform::get_rotation() const
    {
        return rotation_;
    }

    glm::mat4 transform::to_matrix() const
    {
        const auto translate = glm::translate(glm::mat4(), get_position());
        const auto rotate = glm::mat4_cast(get_rotation());
        const auto scale = glm::scale(glm::mat4(), get_scale());

        return translate * rotate * scale;
    }

    transform::operator glm::mat4() const
    {
        return to_matrix();
    }

    void transform::set_position(const glm::vec3& position)
    {
        position_ = position;
    }

    void transform::set_scale(const glm::vec3& scale)
    {
        scale_ = scale;
    }

    void transform::set_rotation(const glm::quat& rotation)
    {
        rotation_ = glm::normalize(rotation);
    }

    glm::vec3 transform::world_front()
    {
        return glm::vec3(0, 0, 1);
    }

    glm::vec3 transform::world_back()
    {
        return -world_front();
    }

    glm::vec3 transform::world_up()
    {
        return glm::vec3(0, 1, 0);
    }

    glm::vec3 transform::world_down()
    {
        return -world_up();
    }

    glm::vec3 transform::world_right()
    {
        return glm::vec3(1, 0, 0);
        ;
    }

    glm::vec3 transform::world_left()
    {
        return -world_right();
    }

    glm::vec3 transform::world_origin()
    {
        return glm::vec3(0, 0, 0);
    }

    glm::vec3 transform::front() const
    {
        return glm::conjugate(rotation_) * world_front();
    }

    glm::vec3 transform::back() const
    {
        return glm::conjugate(rotation_) * world_back();
    }

    glm::vec3 transform::right() const
    {
        return glm::conjugate(rotation_) * world_right();
    }

    glm::vec3 transform::left() const
    {
        return glm::conjugate(rotation_) * world_left();
    }

    glm::vec3 transform::up() const
    {
        return glm::conjugate(rotation_) * world_up();
    }

    glm::vec3 transform::down() const
    {
        return glm::conjugate(rotation_) * world_down();
    }

    void transform::rotate_around(const glm::vec3& point, const glm::quat& rotation)
    {
        auto diff = position_ - point;
        diff = rotation * diff;
        position_ = point + diff;
    }

    void transform::rotate_around(const glm::vec3& point, const glm::vec3& axis, const float angle)
    {
        rotate_around(point, glm::angleAxis(angle, axis));
    }

    void transform::look_at(const glm::vec3& world_location, const glm::vec3& world_up)
    {
        rotation_ = glm::quat_cast(glm::lookAt(-position_, world_location, world_up));
    }

    transform transform::from_matrix(const glm::mat4& m)
    {
        transform t;

        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(m, scale, rotation, translation, skew, perspective);
        rotation = glm::conjugate(rotation);

        t.set_position(translation);
        t.set_scale(scale);
        t.set_rotation(rotation);

        return t;
    }
} // namespace moka
