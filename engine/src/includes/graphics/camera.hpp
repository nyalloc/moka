#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <graphics/transform.hpp>
#include "imgui.h"

#undef near
#undef far

namespace moka
{
	class frustrum
	{

	};

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

		virtual void update(const float delta_time) = 0;
	};

	class camera;

	// a very basic camera - contains only the necessary data members to get things drawing on your screen.
	// can be used as the base concrete type of a decorator-based camera
	class basic_camera : public base_camera
	{
		transform transform_;
		glm::mat4 projection_;
	public:
		basic_camera() = default;

		basic_camera(
			const transform& transform
			, const glm::mat4& projection)
			: transform_(transform)
			, projection_(projection)
		{}

		void update(const float delta_time) override
		{

		}

		glm::mat4 get_view() const override
		{
			return glm::lookAt(transform_.get_position(),
				transform_.get_position() + transform_.front(),
				transform_.up());
		}

		const glm::quat& get_rotation() const override
		{
			return transform_.get_rotation();
		}

		void set_rotation(const glm::quat& rotation) override
		{
			return transform_.set_rotation(rotation);
		}

		const glm::vec3& get_position() const override
		{
			return transform_.get_position();
		}

		void set_position(const glm::vec3& position) override
		{
			return transform_.set_position(position);
		}

		const glm::mat4& get_projection() const override
		{
			return projection_;
		}

		void set_projection(const glm::mat4& projection) override
		{
			projection_ = projection;
		}

		void set_transform(const transform& transform) override
		{
			transform_ = transform;
		}

		const transform& get_transform() const override
		{
			return transform_;
		}
	};

	// camera builder provides a convinient interface to initialising and building a camera
	// cameras can have a lot of different values to set before they are useful
	// having a class like this that can build a complex camera using a few chained function calls is handy!
	class camera_builder
	{
		std::unique_ptr<base_camera> camera_;

	public:
		camera_builder()
			: camera_(std::make_unique<basic_camera>())
		{}

		operator camera();

		camera_builder& set_perspective(
			const float fov
			, const float aspect
			, const float near = 0.1f
			, const float far = 100.0f)
		{
			camera_->set_projection(glm::perspective(fov, aspect, near, far));
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
			camera_->set_projection(glm::ortho(left, right, bottom, top, near, far));
			return *this;
		}

		camera_builder& set_rotation(
			const glm::quat& rotation)
		{
			camera_->set_rotation(rotation);
			return *this;
		}

		camera_builder& set_position(
			const glm::vec3& position)
		{
			camera_->set_position(position);
			return *this;
		}

		camera_builder& set_transform(
			const transform& transform)
		{
			camera_->set_transform(transform);
			return *this;
		}

		camera_builder& set_view_target(
			const glm::vec3& world_location
			, const glm::vec3& world_up = transform::world_up())
		{
			auto transform = camera_->get_transform();
			transform.look_at(world_location, world_up);
			camera_->set_transform(transform);

			return *this;
		}

		camera_builder& set_mouse_controller(mouse& mouse);

		camera build();
	};

	// A camera. This wrapper class provides a simple wrapper interface to access any camera object.
	// An example of the bridge pattern, this abstraction can vary independently of its implementation.
	class camera
	{
		std::unique_ptr<base_camera> camera_;
	public:
		virtual ~camera() = default;

		using builder = camera_builder;

		camera(const camera& camera) = delete;

		camera(camera&& camera) noexcept
			: camera_(std::move(camera.camera_))
		{}

		camera& operator = (const camera& camera) = delete;
		camera& operator = (camera&& camera) = delete;

		camera(std::unique_ptr<base_camera>&& camera)
			: camera_(std::move(camera))
		{}

		glm::mat4 get_view() const
		{
			return camera_->get_view();
		}

		const glm::quat& get_rotation() const
		{
			return camera_->get_rotation();
		}

		const glm::vec3& get_position() const
		{
			return camera_->get_position();
		}

		const glm::mat4& get_projection() const
		{
			return camera_->get_projection();
		}

		const transform& get_transform() const
		{
			return camera_->get_transform();
		}

		void update(const float delta_time) const
		{
			return camera_->update(delta_time);
		}
	};

	inline camera camera_builder::build()
	{
		return { std::move(camera_) };
	}

	// camera decorator. subclass this to add extra functionality to a camera
	class camera_decorator : public base_camera
	{
	protected:
		std::unique_ptr<base_camera> camera_;
	public:

		camera_decorator(
			std::unique_ptr<base_camera>&& camera)
			: camera_(std::move(camera))
		{}

		void update(const float delta_time) override
		{

		}

		glm::mat4 get_view() const override
		{
			return camera_->get_view();
		}

		const glm::quat& get_rotation() const override
		{
			return camera_->get_rotation();
		}

		void set_rotation(const glm::quat& rotation) override
		{
			camera_->set_rotation(rotation);
		}

		const glm::vec3& get_position() const override
		{
			return camera_->get_position();
		}

		void set_position(const glm::vec3& position) override
		{
			camera_->set_position(position);
		}

		const glm::mat4& get_projection() const override
		{
			return camera_->get_projection();
		}

		void set_projection(const glm::mat4& projection) override
		{
			camera_->set_projection(projection);
		}

		void set_transform(const transform& transform) override
		{
			camera_->set_transform(transform);
		}

		const transform& get_transform() const override
		{
			return camera_->get_transform();
		}
	};

	class camera_mouse_controller : public camera_decorator
	{
		mouse& mouse_;

		float current_translate_z = -1.0f;
		float current_rotate_x = -glm::pi<float>();
		float current_rotate_y = 0.0f;

		float translate_z = -1.0f;
		float rotate_x = -glm::pi<float>();
		float rotate_y = 0.0f;
	public:
		camera_mouse_controller(const camera_mouse_controller& camera) = delete;
		camera_mouse_controller(camera_mouse_controller&& camera) = delete;
		camera_mouse_controller& operator = (const camera_mouse_controller& camera) = delete;
		camera_mouse_controller& operator = (camera_mouse_controller&& camera) = delete;

		camera_mouse_controller(
			std::unique_ptr<base_camera>&& camera
			, mouse& mouse)
			: camera_decorator(std::move(camera))
			, mouse_(mouse)
		{
		}

		glm::mat4 get_view() const override
		{
			return camera_->get_view();
		}

		void update(const float delta_time) override
		{
			auto mouse_state = mouse_.get_state();
			auto motion = mouse_state.get_motion();

			const auto& io = ImGui::GetIO();

			if(mouse_state.is_button_down(mouse_button::left) && !io.WantCaptureMouse)
			{
				rotate_x -= motion.x * delta_time;
				rotate_y -= motion.y * delta_time;
			}
			else if (mouse_state.is_button_down(mouse_button::right) && !io.WantCaptureMouse)
			{
				translate_z -= motion.y * delta_time;
			}

			rotate_y = glm::clamp(rotate_y, glm::radians(-89.0f), glm::radians(89.0f));
			translate_z = glm::clamp(translate_z, 0.3f, 5.0f);

			current_rotate_x = glm::mix(current_rotate_x, rotate_x, delta_time * 5);
			current_rotate_y = glm::mix(current_rotate_y, rotate_y, delta_time * 5);
			current_translate_z = glm::mix(current_translate_z, translate_z, delta_time * 5);

			auto trans = get_transform();

			trans.set_position(glm::vec3{ 0, 0, current_translate_z });
			trans.rotate_around(transform::world_origin(), transform::world_right(), current_rotate_y);
			trans.rotate_around(transform::world_origin(), transform::world_up(), current_rotate_x);

			trans.look_at(transform::world_origin());

			set_transform(trans);
		}
	};

	inline camera_builder& camera_builder::set_mouse_controller(mouse& mouse)
	{
		camera_ = std::make_unique<camera_mouse_controller>(std::move(camera_), mouse);
		return *this;
	}

	inline camera_builder::operator camera()
	{
		return this->build();
	}
}