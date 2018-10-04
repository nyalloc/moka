
#include <application/application.hpp>
#include <graphics/graphics_device.hpp>
#include <maths/vector2.hpp>
#include <maths/vector3.hpp>
#include <maths/vector4.hpp>
#include <asset_importer/texture_importer.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <graphics/camera.hpp>
#include <asset_importer/model_importer.hpp>
#include <graphics/model.hpp>
#include <graphics/draw_call_builder.hpp>
#include <graphics/material.hpp>
#include <sstream>

using namespace moka;

class model_loading_application : public app
{
	camera camera_;

	asset_importer<model> model_importer_;

	model model_;
public:

	model_loading_application(const app_settings& settings)
		: app(settings)
		, camera_(camera::builder()
			.set_perspective(glm::radians(70.0f), window_.aspect(), 0.1f, 100.0f)
			.set_position(glm::vec3(0.0f, 0.0f, -1.0f))
			.set_view_target(glm::vec3{ 0 })
			.build())
		, model_importer_(data_path(), graphics_)
		, model_(model_importer_.load("flight_helmet.moka"))
	{}

	~model_loading_application()
	{
		timer_.stop();
	}

	void draw(const game_time delta_time) override
	{
		for(auto& mesh : model_)
		{
			for (auto& primitive : mesh)
			{
				auto& material = primitive.get_material();

				material["model"]      = mesh.get_transform();
				material["view"]       = camera_.get_view();
				material["projection"] = camera_.get_projection();
				material["view_pos"]   = camera_.get_position();

				primitive.draw(graphics_);
			}
		}

		graphics_.frame();
	}

	void update(const game_time delta_time) override
	{
		camera_.update();

		auto normalize = [](const glm::vec3& vector)
		{
			if (vector != glm::vec3{ 0 })
			{
				return glm::normalize(vector);
			}
			return glm::vec3{ 0 };
		};

		auto& camera_transform = camera_.get_transform();

		const auto& key_state = keyboard_.get_state();

		glm::vec3 movement_direction;

		if (key_state.is_key_down(key::key_w))
		{
			movement_direction += camera_transform.front();
		}

		if (key_state.is_key_down(key::key_s))
		{
			movement_direction += camera_transform.back();
		}

		if (key_state.is_key_down(key::key_a))
		{
			movement_direction += camera_transform.left();
		}

		if (key_state.is_key_down(key::key_d))
		{
			movement_direction += camera_transform.right();
		}

		if (key_state.is_key_down(key::up))
		{
			movement_direction += camera_transform.up();
		}

		if (key_state.is_key_down(key::down))
		{
			movement_direction += camera_transform.down();
		}

		const auto& current_pos = camera_transform.get_position();
		auto target_pos = glm::mix(current_pos, current_pos + normalize(movement_direction * delta_time), delta_time);

		camera_transform.set_position(target_pos);

		camera_transform.look_at(glm::vec3{ 0 });
	}

	std::filesystem::path data_path() override
	{
		// cmake-defined macro points to example project asset folder relative to source.
		// A real application could point this wherever it wanted.
		std::filesystem::path result{ MOKA_ASSET_PATH };
		return result.lexically_normal();
	}
};

int main(const int argc, char* argv[])
{
	app_settings settings(argc, argv);
	return model_loading_application{ settings }.run();
}