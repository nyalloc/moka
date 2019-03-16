#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace moka
{
    /**
     * \brief A transform class enclosing a position, scale and rotation.
     */
    class transform final
    {
        glm::vec3 position_;
        glm::vec3 scale_;
        glm::quat rotation_;

    public:
        /**
         * \brief Create a transform object.
         * \param position The transform position.
         * \param scale The transform scale.
         * \param rotation The transform rotation.
         */
        transform(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation);

        transform();

        transform(const transform& rhs);

        transform& operator=(const transform& rhs);

        transform(transform&& rhs) noexcept;

        transform& operator=(transform&& rhs) noexcept;

        ~transform() = default;

        /**
         * \brief Get the transform's local position.
         * \return The transform's local position.
         */
        const glm::vec3& get_position() const;

        /**
         * \brief Get the transform's world position.
         * \return The transform's world position.
         */
        glm::vec3 get_world_position() const;

        /**
         * \brief Get the transform's local scale.
         * \return The transform's local scale.
         */
        const glm::vec3& get_scale() const;

        /**
         * \brief Get the transform's local rotation.
         * \return The transform's local rotation.
         */
        const glm::quat& get_rotation() const;

        /**
         * \brief Convert this transform to a mat4.
         * \return A mat4 defining the transform.
         */
        glm::mat4 to_matrix() const;

        /**
         * \brief Convert this transform to a mat4.
         */
        explicit operator glm::mat4() const;

        /**
         * \brief Set this transform's position.
         * \param position The new position.
         */
        void set_position(const glm::vec3& position);

        /**
         * \brief Set this transform's scale.
         * \param scale The new scale.
         */
        void set_scale(const glm::vec3& scale);

        /**
         * \brief Set this transform's rotation.
         * \param rotation The new rotation.
         */
        void set_rotation(const glm::quat& rotation);

        /**
         * \brief Get the world front direction.
         * \return The world front direction.
         */
        static glm::vec3 world_front();

        /**
         * \brief Get the world back direction.
         * \return The world back direction.
         */
        static glm::vec3 world_back();

        /**
         * \brief Get the world up direction.
         * \return The world up direction.
         */
        static glm::vec3 world_up();

        /**
         * \brief Get the world down direction.
         * \return The world down direction.
         */
        static glm::vec3 world_down();

        /**
         * \brief Get the world right direction.
         * \return The world right direction.
         */
        static glm::vec3 world_right();

        /**
         * \brief Get the world left direction.
         * \return The world left direction.
         */
        static glm::vec3 world_left();

        /**
         * \brief Get the world origin.
         * \return The world origin.
         */
        static glm::vec3 world_origin();

        /**
         * \brief Get the transform's front direction.
         * \return The transform's front direction.
         */
        glm::vec3 front() const;

        /**
         * \brief Get the transform's back direction.
         * \return The transform's back direction.
         */
        glm::vec3 back() const;

        /**
         * \brief Get the transform's right direction.
         * \return The transform's right direction.
         */
        glm::vec3 right() const;

        /**
         * \brief Get the transform's left direction.
         * \return The transform's left direction.
         */
        glm::vec3 left() const;

        /**
         * \brief Get the transform's up direction.
         * \return The transform's up direction.
         */
        glm::vec3 up() const;

        /**
         * \brief Get the transform's down direction.
         * \return The transform's down direction.
         */
        glm::vec3 down() const;

        /**
         * \brief Rotate this transform around a point.
         * \param point The point you want to rotate around.
         * \param rotation The rotation.
         */
        void rotate_around(const glm::vec3& point, const glm::quat& rotation);

        /**
         * \brief Rotate this transform around a point.
         * \param point The point you want to rotate around.
         * \param axis The rotation axis.
         * \param angle The rotation angle.
         */
        void rotate_around(const glm::vec3& point, const glm::vec3& axis, float angle);

        /**
         * \brief Make the transform look at a point in world space.
         * \param world_location The point you want this transform to look at.
         * \param world_up The world up direction.
         */
        void look_at(const glm::vec3& world_location, const glm::vec3& world_up = transform::world_up());

        /**
         * \brief Generate a transform from a matrix.
         * \param m The matrix you want to turn into a transform.
         * \return The new transform object.
         */
        static transform from_matrix(const glm::mat4& m);
    };
} // namespace moka
