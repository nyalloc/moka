
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

struct directional_light
{
	float ambient[3]  { 0.2f, 0.2f, 0.2f };
	float position[3] { 0.0f, 0.5f, 0.5f };
	float diffuse[3]  { 0.8f, 0.8f, 0.8f };
	float specular[3] { 1.0f, 1.0f, 1.0f };
};

struct sort_key_generator
{
	// stolen from http://aras-p.info/blog/2014/01/16/rough-sorting-by-depth/
	static uint32_t depth_to_bits(const float depth)
	{
		union { float f = 0.0f; uint32_t i; } data {};
		data.f = depth;
		return data.i >> 22; // take highest 10 bits
	}

	// http://realtimecollisiondetection.net/blog/?p=86
	sort_key operator()(const float depth, const uint16_t material_id, const alpha_mode alpha) const
	{
		// sort by alpha, then by depth, then by material

		return
			static_cast<sort_key>(material_id)                      |
			static_cast<sort_key>(depth_to_bits(depth))       << 16 | 
			static_cast<sort_key>(alpha == alpha_mode::blend) << 48;
	}
};

class model_loading_application : public app
{
	camera camera_;

	asset_importer<model> model_importer_;

	model model_;

	imgui imgui_;

	float col[4] { 0.8f, 0.8f, 0.8f, 1.0f };

	directional_light light;
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
			.set_color(col[0], col[1], col[2], col[3])
			.set_clear_color(true)
			.set_clear_depth(true);

		for (auto& mesh : model_)
		{
			for (auto& primitive : mesh)
			{
				sort_key_generator gen;

				auto& material = primitive.get_material();

				const auto distance = glm::distance(mesh.get_transform().get_position(), camera_.get_position());

				material["model"] = mesh.get_transform();
				material["view"] = camera_.get_view();
				material["projection"] = camera_.get_projection();
				material["view_pos"] = camera_.get_position();

				material["light.ambient"] = glm::vec3(light.ambient[0], light.ambient[1], light.ambient[2]);
				material["light.position"] = glm::vec3(light.position[0], light.position[1], light.position[2]);
				material["light.diffuse"] = glm::vec3(light.diffuse[0], light.diffuse[1], light.diffuse[2]);
				material["light.specular"] = glm::vec3(light.specular[0], light.specular[1], light.specular[2]);

				primitive.draw(
					scene_draw.make_command_buffer(
						gen(distance, material.get_program().id, material.get_alpha_mode())));
			}
		}

		graphics_.submit(std::move(scene_draw));

		imgui_.new_frame(delta_time);

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ImGui::Begin("Scene");
		ImGui::ColorEdit4("Clear Color", col);
		ImGui::Separator();
		ImGui::Text("Directional Light");
		ImGui::ColorEdit3("Ambient", light.ambient);
		ImGui::ColorEdit3("Diffuse", light.diffuse);
		ImGui::ColorEdit3("Specular", light.specular);
		ImGui::DragFloat3("Direction", light.position, 0.01, -1, 1);
		ImGui::End();

		glm::vec3 pos = { light.position[0], light.position[1], light.position[2] };

		pos = glm::normalize(pos);

		light.position[0] = pos.x;
		light.position[1] = pos.y;
		light.position[2] = pos.z;

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