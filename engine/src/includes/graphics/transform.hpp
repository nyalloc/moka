#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

namespace moka
{
    class transform final
    {
        glm::vec3 position_;
        glm::vec3 scale_;
        glm::quat rotation_;
        bool dirty_; // used for recalculating transform hierarchies!

    public:
        transform(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation)
            : position_(position),
              scale_(scale),
              rotation_(glm::normalize(rotation)),
              dirty_(false)
        {
        }

        transform()
            : position_{0.0f, 0.0f, 0.0f}, scale_{1.0f, 1.0f, 1.0f}, rotation_{1.0f, glm::vec3()}, dirty_(false)
        {
        }

        transform(const transform& rhs) = default;
        transform& operator=(const transform& rhs) = default;

        transform(transform&& rhs) noexcept = default;
        transform& operator=(transform&& rhs) = default;

        ~transform() = default;

        bool is_dirty() const
        {
            return dirty_;
        }

        void set_dirty(const bool dirty)
        {
            dirty_ = dirty;
        }

        const glm::vec3& get_position() const
        {
            return position_;
        }

        const glm::vec3& get_scale() const
        {
            return scale_;
        }

        const glm::quat& get_rotation() const
        {
            return rotation_;
        }

        glm::mat4 to_matrix() const
        {
            const auto translate = glm::translate(glm::mat4(), get_position());
            const auto rotate = glm::mat4_cast(get_rotation());
            const auto scale = glm::scale(glm::mat4(), get_scale());

            return translate * rotate * scale;
        }

        explicit operator glm::mat4() const
        {
            return to_matrix();
        }

        void set_position(const glm::vec3& position)
        {
            position_ = position;
            dirty_ = true;
        }

        void set_scale(const glm::vec3& scale)
        {
            scale_ = scale;
            dirty_ = true;
        }

        void set_rotation(const glm::quat& rotation)
        {
            rotation_ = glm::normalize(rotation);
            dirty_ = true;
        }

        static glm::vec3 world_front()
        {
            return glm::vec3(0, 0, 1);
        }

        static glm::vec3 world_back()
        {
            return -world_front();
        }

        static glm::vec3 world_up()
        {
            return glm::vec3(0, 1, 0);
        }

        static glm::vec3 world_down()
        {
            return -world_up();
        }

        static glm::vec3 world_right()
        {
            return glm::vec3(1, 0, 0);
            ;
        }

        static glm::vec3 world_left()
        {
            return -world_right();
        }

        static glm::vec3 world_origin()
        {
            return glm::vec3(0, 0, 0);
        }

        glm::vec3 front() const
        {
            return glm::conjugate(rotation_) * world_front();
        }

        glm::vec3 back() const
        {
            return glm::conjugate(rotation_) * world_back();
        }

        glm::vec3 right() const
        {
            return glm::conjugate(rotation_) * world_right();
        }

        glm::vec3 left() const
        {
            return glm::conjugate(rotation_) * world_left();
        }

        glm::vec3 up() const
        {
            return glm::conjugate(rotation_) * world_up();
        }

        glm::vec3 down() const
        {
            return glm::conjugate(rotation_) * world_down();
        }

        void rotate_around(const glm::vec3& point, const glm::quat& rotation)
        {
            auto diff = position_ - point;
            diff = rotation * diff;
            position_ = point + diff;
            dirty_ = true;
        }

        void rotate_around(const glm::vec3& point, const glm::vec3& axis, const float angle)
        {
            rotate_around(point, glm::angleAxis(angle, axis));
        }

        void look_at(const glm::vec3& world_location, const glm::vec3& world_up = transform::world_up())
        {
            rotation_ = glm::quat_cast(glm::lookAt(-position_, world_location, world_up));
            dirty_ = true;
        }

        static transform from_matrix(const glm::mat4& m)
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
    };
} // namespace moka
