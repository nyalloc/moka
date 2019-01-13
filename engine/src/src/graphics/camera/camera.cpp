#include <graphics/camera/camera.hpp>

namespace moka
{
	camera::~camera() = default;

	camera::camera(camera&& camera) noexcept: camera_(std::move(camera.camera_))
	{
	}

	camera::camera(std::unique_ptr<base_camera>&& camera): camera_(std::move(camera))
	{
	}

	glm::mat4 camera::get_view() const
	{
		return camera_->get_view();
	}

	const glm::quat& camera::get_rotation() const
	{
		return camera_->get_rotation();
	}

	const glm::vec3& camera::get_position() const
	{
		return camera_->get_position();
	}

	const glm::mat4& camera::get_projection() const
	{
		return camera_->get_projection();
	}

	const transform& camera::get_transform() const
	{
		return camera_->get_transform();
	}

	void camera::update(const float delta_time) const
	{
		return camera_->update(delta_time);
	}
}
