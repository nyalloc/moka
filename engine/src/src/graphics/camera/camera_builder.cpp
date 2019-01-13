#include <graphics/camera/camera_builder.hpp>
#include <graphics/camera/camera.hpp>
#include <graphics/camera/basic_camera.hpp>
#include <input/mouse.hpp>

namespace moka
{
	camera_builder::camera_builder()
        : camera_(std::make_unique<basic_camera>())
	{}

	camera_builder& camera_builder::set_perspective(
        const float y_fov,
        const float aspect, 
        const float z_near,
        const float z_far)
	{
		camera_->set_projection(glm::perspective(y_fov, aspect, z_near, z_far));
		return *this;
	}

	camera_builder& camera_builder::set_orthographic(
        const float left,
		const float right, 
		const float bottom,
        const float top,
		const float z_near,
		const float z_far)
	{
		camera_->set_projection(glm::ortho(left, right, bottom, top, z_near, z_far));
		return *this;
	}

	camera_builder& camera_builder::set_rotation(
		const glm::quat& rotation)
	{
		camera_->set_rotation(rotation);
		return *this;
	}

	camera_builder& camera_builder::set_position(
		const glm::vec3& position)
	{
		camera_->set_position(position);
		return *this;
	}

	camera_builder& camera_builder::set_transform(const transform& transform)
	{
		camera_->set_transform(transform);
		return *this;
	}

	camera_builder& camera_builder::set_view_target(const glm::vec3& world_location, const glm::vec3& world_up)
	{
		auto transform = camera_->get_transform();
		transform.look_at(world_location, world_up);
		camera_->set_transform(transform);

		return *this;
	}

	camera camera_builder::build()
	{
		return {std::move(camera_)};
	}

	camera_builder& camera_builder::set_mouse_controller(mouse& mouse)
	{
		camera_ = std::make_unique<camera_mouse_controller>(std::move(camera_), mouse);
		return *this;
	}
}
