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

	// camera interface. defines all functionality common to moka camera classes
	class base_camera
	{
	public:
		virtual ~base_camera() = default;

		virtual const glm::mat4 get_view() const = 0;

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
		frustrum frustum;

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

		const glm::mat4 get_view() const override
		{
			glm::mat4 matrix = glm::mat4_cast(transform_.get_rotation());
			matrix = glm::translate(matrix, -transform_.get_position());
			return matrix;
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

		void set_transform(const transform& transform)
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

		camera_builder& set_fps_controls(keyboard& keyboard, mouse& mouse);

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

		camera(camera&& rhs)
			: camera_(std::move(rhs.camera_))
		{}

		camera(std::unique_ptr<base_camera>&& camera)
			: camera_(std::move(camera))
		{}

		const glm::mat4 get_view() const
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

		void update(const float delta_time)
		{
			return camera_->update(delta_time);
		}
	};

	camera camera_builder::build()
	{
		return { std::move(camera_) };
	}

	// camera decorator. subclass this to add extra functionality to a camera
	class camera_decorator : public base_camera
	{
	protected:
		std::unique_ptr<base_camera> camera_;
	public:
		camera_decorator() = default;

		camera_decorator(
			std::unique_ptr<base_camera>&& camera)
			: camera_(std::move(camera))
		{}

		void update(const float delta_time) override
		{

		}

		const glm::mat4 get_view() const override
		{
			return camera_->get_view();
		}

		const glm::quat& get_rotation() const override
		{
			return camera_->get_view();
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

		void set_transform(const transform& transform)
		{
			camera_->set_transform(transform);
		}

		const transform& get_transform() const override
		{
			return camera_->get_transform();
		}
	};

	// first person camera controls. 
	// a camera decorator, this class takes a camera and provides basic FPS controls to move it around.
	class camera_fps_controller : public camera_decorator
	{
		keyboard& keyboard_;
		mouse& mouse_;

		float pitch_ = 0.0f;
		float yaw_ = 0.0f;

		glm::quat target_rot;
		glm::vec3 target_pos;

		static constexpr float max_pitch_ = glm::radians(89.0f);
		static constexpr float movement_interpolant_ = 2.00f;
		static constexpr float rotation_interpolant_ = 10.0f;
	public:
		camera_fps_controller() = default;

		camera_fps_controller(
			std::unique_ptr<base_camera>&& camera
			, keyboard& keyboard
			, mouse& mouse)
			: camera_decorator(std::move(camera))
			, keyboard_(keyboard)
			, mouse_(mouse)
		{
			auto camera_transform = get_transform();
			auto euler_angles = glm::eulerAngles(camera_transform.get_rotation());
			pitch_ = euler_angles.x;
			yaw_ = euler_angles.y;
		}

		void update(const float delta_time) override
		{
			auto normalize = [](const glm::vec3& vector)
			{
				if (vector != glm::vec3{ 0 })
				{
					return glm::normalize(vector);
				}
				return glm::vec3{ 0 };
			};

			auto camera_transform = get_transform();

			const auto& key_state = keyboard_.get_state();

			glm::vec3 movement_direction;

			if (key_state.is_key_down(key::w))
			{
				movement_direction += camera_transform.back();
			}

			if (key_state.is_key_down(key::s))
			{
				movement_direction += camera_transform.front();
			}

			if (key_state.is_key_down(key::a))
			{
				movement_direction += camera_transform.left();
			}

			if (key_state.is_key_down(key::d))
			{
				movement_direction += camera_transform.right();
			}

			if (key_state.is_key_down(key::up))
			{
				movement_direction += camera_transform.world_up();
			}

			if (key_state.is_key_down(key::down))
			{
				movement_direction += camera_transform.world_down();
			}

			auto direction = normalize(movement_direction);

			auto target_pos = camera_transform.get_position() + direction;

			const auto& motion = mouse_.get_state().get_motion() * delta_time;

			yaw_ += motion.x;
			pitch_ = glm::clamp(pitch_ + motion.y, -max_pitch_, max_pitch_);

			auto yaw_quat = glm::angleAxis(yaw_, glm::vec3(0.0f, 1.0f, 0.0f));
			auto pitch_quat = glm::angleAxis(pitch_, glm::vec3(1.0f, 0.0f, 0.0f));

			target_rot = pitch_quat * yaw_quat;

			camera_transform.set_position(glm::mix(camera_transform.get_position(), target_pos, delta_time * movement_interpolant_));
			
			camera_transform.set_rotation(glm::lerp(camera_transform.get_rotation(), target_rot, delta_time * rotation_interpolant_));

			camera_->set_transform(camera_transform);

			camera_->update(delta_time);
		}
	};

	camera_builder& camera_builder::set_fps_controls(keyboard& keyboard, mouse& mouse)
	{
		camera_ = std::make_unique<camera_fps_controller>(std::move(camera_), keyboard, mouse);
		return *this;
	}

	camera_builder::operator camera()
	{
		return this->build();
	}
}