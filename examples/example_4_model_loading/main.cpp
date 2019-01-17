
#include <application/application.hpp>
#include <application/open_file.hpp>
#include <asset_importer/model_importer.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <graphics/camera/camera.hpp>
#include <graphics/device/graphics_device.hpp>
#include <graphics/material/material_builder.hpp>
#include <graphics/model.hpp>
#include <imgui.h>
#include <imgui.hpp>

using namespace moka;

struct directional_light final
{
    glm::vec3 ambient = colour::light_slate_grey();
    glm::vec3 direction = {0, 0, -1};
    glm::vec3 diffuse = colour::dim_grey();
    glm::vec3 specular = colour::white();
};

model make_cube(graphics_device& device)
{
    // clang-format off
    float vertices[] = 
    {
        // back face
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // front face
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        // right face
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
        // bottom face
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
        // top face
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
         1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
    };
    // clang-format on

    const auto size = sizeof(float);

    auto layout =
        vertex_layout::builder{}
            .add_attribute(0, attribute_type::float32, 3, false, 8 * size, 0)
            .add_attribute(1, attribute_type::float32, 3, false, 8 * size, 3 * size)
            .add_attribute(2, attribute_type::float32, 2, false, 8 * size, 6 * size)
            .build();

    auto buffer = device.make_vertex_buffer(
        vertices, sizeof vertices, std::move(layout), buffer_usage::static_draw);

    std::map<std::string, program> shaders;

    auto material = material::builder{device, shaders}
                        .add_uniform("projection", glm::mat4{})
                        .add_uniform("view", glm::mat4{})
                        .add_uniform("equirectangularMap", texture{})
                        .build();

    primitive primitive(buffer, sizeof vertices / sizeof(float), std::move(material));

    mesh mesh(primitive);

    return model{mesh};
}

class app final : public application
{
    camera camera_;

    asset_importer<model> model_importer_;

    rectangle viewport_ = {0, 0, 1280, 720};

    model model_;

    model cube_;

    texture hdr_{};

    glm::vec4 color_{0.8f, 0.8f, 0.8f, 1.0f};

    directional_light light_;

    imgui imgui_;

public:
    explicit app(const app_settings& settings)
        : application(settings),
          camera_(camera::builder{}
                      .set_mouse_controller(mouse_)
                      .set_perspective(glm::radians(70.0f), window_.aspect())
                      .build()),
          model_importer_(app::data_path(), graphics_),
          model_(model_importer_.load(
              app::data_path() / "Models" / "FlightHelmet" / "FlightHelmet.gltf",
              app::data_path() / "Materials" / "pbr.material")),
          cube_(model_importer_.load(
              app::data_path() / "Models" / "Cube" / "Cube.gltf",
              app::data_path() / "Materials" / "cube.material")),
          imgui_(window_, keyboard_, mouse_, graphics_)
    {
        auto height = 0;
        auto width = 0;
        base_pixel_format format;

        const auto path = app::data_path() / "Textures" / "siggraph.hdr";

        const auto data = texture_load_hdr(path, width, height, format);

        const texture_wrap_mode wrap{wrap_mode::clamp_to_edge, wrap_mode::clamp_to_edge};

        const texture_filter_mode filter{mag_filter_mode::linear, min_filter_mode::linear};

        hdr_ = graphics_.make_texture(
            texture_target::texture_2d,
            data,
            texture_type::float32,
            width,
            height,
            format,
            internal_pixel_format::rgb16f,
            filter,
            wrap,
            false,
            true);
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
            ImGui::ColorEdit3("Ambient", &light_.ambient[0]);
            ImGui::ColorEdit3("Diffuse", &light_.diffuse[0]);
            ImGui::ColorEdit3("Specular", &light_.specular[0]);
            ImGui::DragFloat3("Direction", &light_.direction[0], 0.01, -1, 1);
        }
        ImGui::End();

        light_.direction = glm::normalize(light_.direction);

        graphics_.submit_and_swap(imgui_.draw());
    }

    void draw(const game_time delta_time) override
    {
        command_list scene_draw;

        scene_draw.viewport().set_rectangle(viewport_);

        scene_draw.scissor().set_rectangle(viewport_);

        scene_draw.clear()
            .set_color(colour::antique_white())
            .set_clear_color(true)
            .set_clear_depth(true);

        for (auto& mesh : model_)
        {
            for (auto& primitive : mesh)
            {
                auto& material = primitive.get_material();

                const auto distance = glm::distance(
                    mesh.get_transform().get_position(), camera_.get_position());

                material["model"] = mesh.get_transform();
                material["view"] = camera_.get_view();
                material["projection"] = camera_.get_projection();
                material["view_pos"] = camera_.get_position();

                material["light.ambient"] = light_.ambient;
                material["light.direction"] = light_.direction;
                material["light.diffuse"] = light_.diffuse;
                material["light.specular"] = light_.specular;

                const auto sort_key = generate_sort_key(
                    distance, material.get_program().id, material.get_alpha_mode());

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
        // cmake-defined macro points to example project asset folder relative
        // to source. A real application could point this wherever it wanted.
        std::filesystem::path result{MOKA_ASSET_PATH};
        return result.lexically_normal();
    }
};

int main(const int argc, char* argv[])
{
    const app_settings settings(argc, argv);
    return app{settings}.run();
}
