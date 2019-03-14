#include <application/application.hpp>
#include <application/open_file.hpp>
#include <asset_importer/model_importer.hpp>
#include <filesystem>
#include <graphics/camera/camera.hpp>
#include <graphics/color.hpp>
#include <graphics/device/graphics_device.hpp>
#include <graphics/material/material_builder.hpp>
#include <graphics/model.hpp>
#include <graphics/pbr.hpp>
#include <imgui.h>
#include <imgui.hpp>

using namespace moka;

class app final : public application
{
    camera camera_;

    asset_importer<model> model_importer_;

    model model_;

    pbr_util pbr_;

    texture hdr_{};

    texture irradiance_{};

    texture brdf_{};

    texture prefiltered_{};

    model cube_;

    glm::vec4 color_ = color::burnt_sienna();

    imgui imgui_;

    float gamma_ = 2.2f;

    float exposure_ = 1.0f;

    bool environment_ = true;

    bool rotate_ = true;

    float fov_ = 70.0f;

public:
    explicit app(const app_settings& settings)
        : application(settings),
          pbr_(graphics_),
          camera_(camera::builder{}
                      .set_mouse_controller(mouse_)
                      .set_perspective(glm::radians(70.0f), window_.aspect())
                      .build()),
          model_importer_(app::data_path(), graphics_),
          model_(model_importer_.load(
              app::data_path() / "Models" / "BoomBox" / "BoomBox.gltf",
              app::data_path() / "Materials" / "pbr.material")),
          hdr_(pbr_.make_hdr_environment_map(app::data_path() / "Textures" / "gym.hdr")),
          irradiance_(pbr_.make_irradiance_environment_map(hdr_)),
          prefiltered_(pbr_.make_specular_environment_map(hdr_)),
          brdf_(pbr_.make_brdf_integration_map()),
          cube_(pbr_.make_skybox(hdr_)),
          imgui_(window_, keyboard_, mouse_, graphics_)
    {
    }

    static uint32_t depth_to_bits(const float depth)
    {
        // stolen from http://aras-p.info/blog/2014/01/16/rough-sorting-by-depth/
        union {
            float f = 0.0f;
            uint32_t i;
        } data{};
        data.f = depth;
        return data.i >> 22; // take highest 10 bits
    }

    static sort_key generate_sort_key(const float depth, const uint16_t material_id, const alpha_mode alpha)
    {
        // http://realtimecollisiondetection.net/blog/?p=86
        // sort by alpha, then by depth, then by material
        return static_cast<sort_key>(material_id) |
               static_cast<sort_key>(depth_to_bits(depth)) << 16 |
               static_cast<sort_key>(alpha == alpha_mode::blend) << 48;
    }

    void draw_imgui(const game_time delta_time)
    {
        imgui_.new_frame(delta_time);

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Options"))
                {
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        auto& io = ImGui::GetIO();

        ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background
        if (ImGui::Begin(
                "Overlay",
                nullptr,
                (0 != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize |
                    ImGuiWindowFlags_NoSavedSettings |
                    ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
        {
            ImGui::Text(
                "Real-Time Physically-Based Rendering\n"
                "Stuart Adams\n"
                "@stuartdadams\n"
                "www.stuartdadams.co.uk");
        }
        ImGui::End();

        ImGui::Begin("Render Settings");
        {
            ImGui::SliderFloat("Gamma", &gamma_, 0.0f, 10.0f, "%.3f");
            ImGui::SliderFloat("Exposure", &exposure_, 0.0f, 10.0f, "%.3f");
            ImGui::SliderFloat(
                "Camera FOV", &fov_, 20.0f, 150.0f, "%.3f degrees");
            ImGui::Checkbox("Camera Auto Rotate", &rotate_);
            ImGui::Checkbox("Draw Environment", &environment_);
            ImGui::ColorEdit4("Clear Color", reinterpret_cast<float*>(&color_), ImGuiColorEditFlags_PickerHueWheel);
        }
        ImGui::End();

        graphics_.submit_and_swap(imgui_.draw());
    }

    void draw(const game_time delta_time) override
    {
        command_list scene_draw;

        const auto viewport = window_.get_viewport();

        scene_draw.viewport().set_rectangle(viewport);

        scene_draw.scissor().set_rectangle(viewport);

        scene_draw.clear().set_color(color_).set_clear_color(true).set_clear_depth(true);

        for (auto& mesh : model_)
        {
            for (auto& primitive : mesh)
            {
                auto material = primitive.get_material();

                const auto distance = glm::distance(
                    mesh.get_transform().get_position(), camera_.get_position());

                scene_draw.set_material_parameters()
                    .set_material(material)
                    .set_parameter("gamma", gamma_)
                    .set_parameter("exposure", exposure_)
                    .set_parameter("irradiance_map", irradiance_)
                    .set_parameter("prefilter_map", prefiltered_)
                    .set_parameter("brdf_lut", brdf_)
                    .set_parameter("model", mesh.get_transform().to_matrix())
                    .set_parameter("view", camera_.get_view())
                    .set_parameter("projection", camera_.get_projection())
                    .set_parameter("view_pos", camera_.get_position());

                auto* mat = graphics_.get_material_cache().get_material(material);

                if (mat)
                {
                    auto key = generate_sort_key(
                        distance, mat->get_program().id, mat->get_alpha_mode());

                    auto& buffer = scene_draw.make_command_buffer(key);

                    primitive.draw(buffer);
                }
            }
        }

        if (environment_)
        {
            for (auto& mesh : cube_)
            {
                for (auto& primitive : mesh)
                {
                    auto material = primitive.get_material();

                    scene_draw.set_material_parameters()
                        .set_material(material)
                        .set_parameter("gamma", gamma_)
                        .set_parameter("exposure", exposure_)
                        .set_parameter("view", camera_.get_view())
                        .set_parameter("projection", camera_.get_projection());

                    auto& buffer = scene_draw.make_command_buffer();

                    primitive.draw(buffer);
                }
            }
        }

        graphics_.submit(std::move(scene_draw));

        draw_imgui(delta_time);
    }

    void update(const game_time delta_time) override
    {
        camera_.set_perspective(glm::radians(fov_), window_.aspect());
        camera_.update(delta_time);
    }

    std::filesystem::path data_path() override
    {
        // cmake-defined macro points to example project asset folder relative
        // to source. A real application could point this wherever it wanted.
        std::filesystem::path result{MOKA_ASSET_PATH};
        return result.lexically_normal();
    }
};

int main()
{
    app_settings settings{};
    settings.window_settings.resolution = {1600, 900};
    settings.window_settings.fullscreen = false;

    return app{settings}.run();
}
