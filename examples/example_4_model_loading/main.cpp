
#include <application/application.hpp>
#include <graphics/device/graphics_device.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <graphics/camera/camera.hpp>
#include <asset_importer/model_importer.hpp>
#include <graphics/model.hpp>
#include <graphics/material/material.hpp>
#include <imgui.hpp>
#include <application/open_file.hpp>

using namespace moka;

struct directional_light
{
	glm::vec3 ambient = color::light_slate_grey();
	glm::vec3 direction = { 0, 0, -1 };
	glm::vec3 diffuse = color::dim_grey();
	glm::vec3 specular = color::white();
};

class model_loading_application : public application
{
	camera camera_;

	asset_importer<model> model_importer_;

	rectangle viewport_ = { 0, 0, 1280, 720 };

	model model_;

	imgui imgui_;

	glm::vec4 color_ { 0.8f, 0.8f, 0.8f, 1.0f };

	directional_light light;
public:

	explicit model_loading_application(const app_settings& settings)
		: application(settings)
		, camera_(camera::builder()
			.set_mouse_controller(mouse_)
			.set_perspective(glm::radians(70.0f), window_.aspect())
			.build())
		, model_importer_(model_loading_application::data_path(), graphics_)
		, model_(model_importer_.load("flight_helmet.moka"))
		, imgui_(window_, keyboard_, mouse_, graphics_)
	{}

	void draw_imgui(const game_time delta_time)
	{
		imgui_.new_frame(delta_time);

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open"))
				{
					open_file();
				}

				if (ImGui::MenuItem("Exit"))
				{

				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Options"))
				{

				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Options"))
				{

				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ImGui::Begin("Scene");
		{
			ImGui::ColorEdit4("Clear Color", &color_[0]);
			ImGui::Separator();
			ImGui::Text("Directional Light");
			ImGui::ColorEdit3("Ambient", &light.ambient[0]);
			ImGui::ColorEdit3("Diffuse", &light.diffuse[0]);
			ImGui::ColorEdit3("Specular", &light.specular[0]);
			ImGui::DragFloat3("Direction", &light.direction[0], 0.01, -1, 1);
		}
		ImGui::End();

		light.direction = glm::normalize(light.direction);

		graphics_.submit_and_swap(imgui_.draw());
	}

	static uint32_t depth_to_bits(const float depth)
	{
		// stolen from http://aras-p.info/blog/2014/01/16/rough-sorting-by-depth/
		union { float f = 0.0f; uint32_t i; } data{};
		data.f = depth;
		return data.i >> 22; // take highest 10 bits
	}

	static sort_key generate_sort_key(const float depth, const uint16_t material_id, const alpha_mode alpha)
	{
		// http://realtimecollisiondetection.net/blog/?p=86
		// sort by alpha, then by depth, then by material
		return
			static_cast<sort_key>(material_id) |
			static_cast<sort_key>(depth_to_bits(depth)) << 16 |
			static_cast<sort_key>(alpha == alpha_mode::blend) << 48;
	}

	void draw(const game_time delta_time) override
	{
		command_list scene_draw;

		scene_draw.viewport()
			.set_rectangle(viewport_);

		scene_draw.scissor()
			.set_rectangle(viewport_);

		scene_draw.clear()
			.set_color(color::antique_white())
			.set_clear_color(true)
			.set_clear_depth(true);

		for (auto& mesh : model_)
		{
			for (auto& primitive : mesh)
			{
				auto& material = primitive.get_material();

				const auto distance = glm::distance(mesh.get_transform().get_position(), camera_.get_position());

				material["model"]           = mesh.get_transform();
				material["view"]            = camera_.get_view();
				material["projection"]      = camera_.get_projection();
				material["view_pos"]        = camera_.get_position();

				material["light.ambient"]   = light.ambient;
				material["light.direction"] = light.direction;
				material["light.diffuse"]   = light.diffuse;
				material["light.specular"]  = light.specular;

				const auto sort_key = generate_sort_key(distance, material.get_program().id, material.get_alpha_mode());

				auto& buffer = scene_draw.make_command_buffer(sort_key);

				primitive.draw(buffer);
			}
		}

		graphics_.submit(std::move(scene_draw));

		draw_imgui(delta_time);
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