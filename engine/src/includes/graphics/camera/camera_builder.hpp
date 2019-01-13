#pragma once
#include <memory>
#include <graphics/camera/base_camera.hpp>
#include <graphics/camera/camera_mouse_controller.hpp>

namespace moka
{
	class mouse;
	class camera;

	// camera builder provides a convenient interface to initializing and building a camera
	// cameras can have a lot of different values to set before they are useful
	// having a class like this that can build a complex camera using a few chained function calls is handy!
	class camera_builder final
	{
		std::unique_ptr<base_camera> camera_;

	public:
		camera_builder();

		camera_builder& set_perspective(
			float y_fov
			, float aspect
			, float z_near = 0.1f
			, float z_far = 100.0f);

		camera_builder& set_orthographic(
			float left
			, float right
			, float bottom
			, float top
			, float z_near
			, float z_far);

		camera_builder& set_rotation(
			const glm::quat& rotation);

		camera_builder& set_position(
			const glm::vec3& position);

		camera_builder& set_transform(
			const transform& transform);

		camera_builder& set_view_target(
			const glm::vec3& world_location
			, const glm::vec3& world_up = transform::world_up());

		camera build();

		camera_builder& set_mouse_controller(mouse& mouse);
	};
}
