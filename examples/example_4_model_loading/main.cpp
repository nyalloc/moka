
#include <application/application.hpp>
#include <graphics/graphics_device.hpp>
#include <maths/vector2.hpp>
#include <maths/vector3.hpp>
#include <maths/vector4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <graphics/camera.hpp>
#include <asset_importer/model_importer.hpp>
#include <graphics/model.hpp>
#include <graphics/material.hpp>
#include <imgui.hpp>

using namespace moka;

struct sort_key_generator
{
	sort_key operator()(const float depth, const uint16_t material_id, const alpha_mode alpha) const
	{
		return
		{
			static_cast<sort_key>(depth * std::numeric_limits<uint16_t>::max()) |
			static_cast<sort_key>(alpha) << 16 |
			static_cast<sort_key>(material_id) << 24
		};
	}
};

class model_loading_application : public app
{
	camera camera_;

	asset_importer<model> model_importer_;

	model model_;

	imgui imgui_;
public:

	model_loading_application(const app_settings& settings)
		: app(settings)
		, camera_(camera::builder()
			.set_mouse_controller(mouse_)
			.set_perspective(glm::radians(70.0f), window_.aspect()))
		, model_importer_(model_loading_application::data_path(), graphics_)
		, model_(model_importer_.load("flight_helmet.moka"))
		, imgui_(window_, keyboard_, mouse_, graphics_)
	{}

	void draw(const game_time delta_time) override
	{
		command_list scene_draw;

		scene_draw.viewport()
			.set_rectangle(0, 0, 1280, 720);

		scene_draw.scissor()
			.set_rectangle(0, 0, 1280, 720);

		scene_draw.clear()
			.set_color(0.6f, 0.6f, 0.6f, 1.0f)
			.set_clear_color(true)
			.set_clear_depth(true);

		for (auto& mesh : model_)
		{
			for (auto& primitive : mesh)
			{
				auto& material = primitive.get_material();

				material["model"] = mesh.get_transform();
				material["view"] = camera_.get_view();
				material["projection"] = camera_.get_projection();
				material["view_pos"] = camera_.get_position();

				primitive.draw(scene_draw);
			}
		}

		graphics_.submit(std::move(scene_draw));

		imgui_.new_frame(delta_time);

		ImGui::ShowDemoWindow();

		graphics_.submit_and_swap(imgui_.draw());
	}

	void update(const game_time delta_time) override
	{
		camera_.update(delta_time);
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
	const app_settings settings(argc, argv);
	return model_loading_application{ settings }.run();
}