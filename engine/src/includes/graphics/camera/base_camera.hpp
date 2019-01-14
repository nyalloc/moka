#pragma once

#include <glm/glm.hpp>
#include <graphics/transform.hpp>

namespace moka
{
	// camera interface. defines all functionality common to moka camera classes
	class base_camera
	{
	public:
		base_camera() = default;
		base_camera(const base_camera& base_camera) = delete;
		base_camera(base_camera&& base_camera) = delete;
		base_camera& operator = (const base_camera& base_camera) = delete;
		base_camera& operator = (base_camera&& base_camera) = delete;

		virtual ~base_camera() = default;

		virtual glm::mat4 get_view() const = 0;

		virtual const glm::quat& get_rotation() const = 0;
		virtual void set_rotation(const glm::quat& rotation) = 0;

		virtual const glm::vec3& get_position() const = 0;
		virtual void set_position(const glm::vec3& position) = 0;

		virtual const glm::mat4& get_projection() const = 0;
		virtual void set_projection(const glm::mat4& projection) = 0;

		virtual const transform& get_transform() const = 0;
		virtual void set_transform(const transform& transform) = 0;

		virtual void update(float delta_time) = 0;
	};
}
