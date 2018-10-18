
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
#include <iostream>
#include <imgui.hpp>

using namespace moka;

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
			.set_fps_controls(keyboard_, mouse_)
			.set_position(glm::vec3(0, 1, 0))
			.set_perspective(glm::radians(70.0f), window_.aspect()))
		, model_importer_(data_path(), graphics_)
		, model_(model_importer_.load("flight_helmet.moka"))
		, imgui_(window_, keyboard_, mouse_, graphics_)
	{
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

		ImGui::NewFrame();

		bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();

		imgui_.draw(ImGui::GetDrawData());

		graphics_.frame();
	}

	void update(const game_time delta_time) override
	{
		camera_.update(delta_time);
		imgui_.update(delta_time);
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