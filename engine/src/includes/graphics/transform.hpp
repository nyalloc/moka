#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/orthonormalize.hpp>

namespace moka
{
	class transform
	{
		glm::vec3 position_;
		glm::vec3 scale_;
		glm::quat rotation_;

	public:
		transform(
			const glm::vec3& position
			, const glm::vec3& scale
			, const glm::quat& rotation)
			: position_(position)
			, scale_(scale)
			, rotation_(rotation)
		{}

		transform()
			: position_{ 0.0f, 0.0f, 0.0f }
			, scale_{ 1.0f, 1.0f, 1.0f }
			, rotation_{ 1.0f, glm::vec3() }
		{}

		transform(const transform& rhs) = default;
		transform& operator = (const transform& rhs) = default;

		transform(transform&& rhs) noexcept = default;
		transform& operator = (transform&& rhs) = default;

		~transform() = default;

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

		void set_position(const glm::vec3& position)
		{
			position_ = position;
		}

		void set_scale(const glm::vec3& scale)
		{
			scale_ = scale;
		}

		void set_rotation(const glm::quat& rotation)
		{
			rotation_ = rotation;
		}

		static glm::vec3 world_front()
		{
			return glm::vec3(0, 0, 1);
		}

		static glm::vec3 world_back()
		{
			return -world_back();
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
			return glm::vec3(1, 0, 0);;
		}

		static glm::vec3 world_left()
		{
			return -world_right();
		}

		glm::vec3 front() const
		{
			return glm::normalize(rotation_ * world_front());
		}

		glm::vec3 back() const
		{
			return -front();
		}

		glm::vec3 right() const
		{
			return glm::normalize(glm::cross(front(), world_up()));
		}

		glm::vec3 left() const
		{
			return -right();
		}

		glm::vec3 up() const
		{
			return glm::normalize(glm::cross(right(), front()));
		}

		glm::vec3 down() const
		{
			return -up();
		}

		void look_at(
			const glm::vec3& world_location
			, const glm::vec3& world_up = world_up())
		{
			glm::mat3 result;

			result[2] = -glm::normalize(position_ - world_location);
			result[0] = glm::normalize(glm::cross(world_up, result[2]));
			result[1] = glm::normalize(glm::cross(result[2], result[0]));

			rotation_ = glm::quat_cast(result);
		}
	};
}