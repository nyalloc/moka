#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/orthonormalize.hpp>
#include <game_loop/game_loop.hpp>
#include <graphics/transform.hpp>

#undef near
#undef far

namespace moka
{
	class frustrum
	{

	};

	class camera;

	class camera_builder
	{
		glm::mat4 projection_;
		glm::mat4 view_;
		transform transform_;

	public:
		camera_builder() = default;

		camera_builder& set_perspective(
			const float fov
			, const float aspect
			, const float near
			, const float far)
		{
			projection_ = glm::perspective(fov, aspect, near, far);
			return *this;
		}

		camera_builder& set_orthographic(
			const float left
			, const float right
			, const float bottom
			, const float top
			, const float near
			, const float far)
		{
			projection_ = glm::ortho(left, right, bottom, top, near, far);
			return *this;
		}

		camera_builder& set_rotation(
			const glm::quat& rotation)
		{
			transform_.set_rotation(rotation);
			return *this;
		}

		camera_builder& set_position(
			const glm::vec3& position)
		{
			transform_.set_position(position);
			return *this;
		}

		camera_builder& set_scale(
			const glm::vec3& scale)
		{
			transform_.set_scale(scale);
			return *this;
		}

		camera_builder& set_transform(
			const transform& transform)
		{
			transform_ = transform;
			return *this;
		}

		camera_builder& set_view_target(
			const glm::vec3& world_location
			, const glm::vec3& world_up = transform::world_up())
		{
			transform_.look_at(world_location, world_up);
			return *this;
		}

		camera build();
	};

	class camera
	{
		transform transform_;
		glm::mat4 projection_;
		glm::mat4 view_;
		frustrum frustum;

		glm::mat4 calculate_view() const
		{
			return glm::lookAt(
				transform_.get_position()
				, transform_.get_position() + transform_.front()
				, transform_.up());
		}
	public:

		using builder = camera_builder;

		camera() = default;

		camera(
			const transform& transform
			, const glm::mat4& projection)
			: transform_(transform)
			, projection_(projection)
			, view_(calculate_view())
		{}

		void update()
		{
			view_ = calculate_view();
		}

		const glm::mat4& get_view() const
		{
			return view_;
		}

		const glm::mat4& get_projection() const
		{
			return projection_;
		}

		void set_transform(const transform& transform)
		{
			transform_ = transform;
		}

		const transform& get_transform() const
		{
			return transform_;
		}

		transform& get_transform()
		{
			return transform_;
		}
	};

	camera camera_builder::build()
	{
		return { transform_, projection_ };
	}
}