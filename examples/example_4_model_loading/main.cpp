#include <application/application.hpp>
#include <application/open_file.hpp>
#include <asset_importer/model_importer.hpp>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <graphics/camera/camera.hpp>
#include <graphics/color.hpp>
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

model make_hdr_environment_map(graphics_device& device, const std::filesystem::path& texture_path)
{
    // we need to take a 2D texture, then turn it into an environment map.
    // to do this, we need to use a shader that will project the 2d texture onto the sides of a cube.
    // we will then render this cube from all 6 directions and capture the result in a frame buffer.
    // we can then use the 6 textures to stitch together our cubemap.

    auto height = 0;
    auto width = 0;
    host_format format;

    const auto data = texture_load_hdr(texture_path, width, height, format);

    const auto hdr = device.build_texture()
                         .add_image_data(
                             image_target::texture_2d,
                             0,
                             device_format::rgb16f,
                             width,
                             height,
                             0,
                             host_format::rgb,
                             pixel_type::float32,
                             data)
                         .set_wrap_s(wrap_mode::clamp_to_edge)
                         .set_wrap_t(wrap_mode::clamp_to_edge)
                         .set_min_filter(min_filter::linear)
                         .set_mag_filter(mag_filter::linear)
                         .free_host_data(true)
                         .build();

    // clang-format off
    float vertices[] = 
    {
        // back face-----------------------------------------------------------|
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, //< bottom-left  |
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, //< top-right    |
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, //< bottom-right |
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, //< top-right    |
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, //< bottom-left  |
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, //< top-left     |
        // front face----------------------------------------------------------|
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, //< bottom-left  |
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, //< bottom-right |
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, //< top-right    |
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, //< top-right    |
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, //< top-left     |
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, //< bottom-left  |
        // left face-----------------------------------------------------------|
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, //< top-right    |
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, //< top-left     |
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, //< bottom-left  |
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, //< bottom-left  |
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, //< bottom-right |
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, //< top-right    |
        // right face----------------------------------------------------------|
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, //< top-left     |
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, //< bottom-right |
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, //< top-right    |
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, //< bottom-right |
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, //< top-left     |
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, //< bottom-left  |
        // bottom face---------------------------------------------------------|
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, //< top-right    |
         1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, //< top-left     |
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, //< bottom-left  |
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, //< bottom-left  |
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, //< bottom-right |
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, //< top-right    |
        // top face------------------------------------------------------------|
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, //< top-left     |
         1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, //< bottom-right |
         1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, //< top-right    |
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, //< bottom-right |
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, //< top-left     |
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  //< bottom-left  |
        // --------------------------------------------------------------------|
    };
    // clang-format on

    const auto size = sizeof(float);

    auto layout =
        vertex_layout::builder{}
            .add_attribute(0, attribute_type::float32, 3, false, 8 * size, 0)
            .add_attribute(1, attribute_type::float32, 3, false, 8 * size, 3 * size)
            .add_attribute(2, attribute_type::float32, 2, false, 8 * size, 6 * size)
            .build();

    const auto buffer = device.make_vertex_buffer(
        vertices, sizeof vertices, std::move(layout), buffer_usage::static_draw);

    std::map<std::string, program> shaders;

    const auto hdr_to_cube_vert = R"(

        #version 330 core

        layout (location = 0) in vec3 a_pos;

        out vec3 local_pos;

        uniform mat4 projection;
        uniform mat4 view;

        void main()
        {
            local_pos = a_pos;  
            gl_Position =  projection * view * vec4(local_pos, 1.0f);
        }
    )";

    const auto hdr_to_cube_frag = R"(

        #version 330 core

        out vec4 frag_color;
        in vec3 local_pos;

        uniform sampler2D map;

        const vec2 inv_atan = vec2(0.1591f, 0.3183f);
        vec2 sample_spherical_map(vec3 v)
        {
            vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
            uv *= inv_atan;
            uv += 0.5f;
            return uv;
        }

        void main()
        {		
            vec2 uv = sample_spherical_map(normalize(local_pos));
            vec3 color = texture(map, uv).rgb;
            
            frag_color = vec4(color, 1.0f);
        }
    )";

    const auto projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

    auto hdr_material = material::builder{device, shaders}
                            .set_vertex_shader(hdr_to_cube_vert)
                            .set_fragment_shader(hdr_to_cube_frag)
                            .add_uniform("projection", projection)
                            .add_uniform("view", parameter_type::mat4)
                            .add_uniform("map", hdr)
                            .set_culling_enabled(false)
                            .build();

    command_list cube_draw;

    glm::mat4 capture_views[] = {
        lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
        lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))};

    image_target image_targets[] = {
        image_target::cubemap_positive_x,
        image_target::cubemap_negative_x,
        image_target::cubemap_positive_y,
        image_target::cubemap_negative_y,
        image_target::cubemap_positive_z,
        image_target::cubemap_negative_z,
    };

    auto texture_builder = device.build_texture();

    for (auto& image_target : image_targets)
    {
        texture_builder.add_image_data(
            image_target, 0, device_format::rgb16f, 512, 512, 0, host_format::rgb, pixel_type::float32, nullptr);
    }

    auto cubemap = texture_builder.set_target(texture_target::cubemap)
                       .set_wrap_s(wrap_mode::clamp_to_edge)
                       .set_wrap_t(wrap_mode::clamp_to_edge)
                       .set_wrap_r(wrap_mode::clamp_to_edge)
                       .set_min_filter(min_filter::linear)
                       .set_mag_filter(mag_filter::linear)
                       .build();

    const auto frame_buffer =
        device.build_frame_buffer()
            .add_depth_attachment(frame_format::depth_component24, 512, 512)
            .build();

    cube_draw.viewport().set_rectangle({0, 0, 512, 512});

    cube_draw.frame_buffer().set_frame_buffer(frame_buffer);

    for (auto i = 0; i < 6; i++)
    {
        hdr_material["view"] = capture_views[i];

        auto image_target = image_targets[i];

        cube_draw.frame_buffer_texture()
            .set_texture(cubemap)
            .set_attachment(frame_attachment::color)
            .set_target(image_target)
            .set_mip_level(0);

        cube_draw.clear().set_clear_color(true).set_clear_depth(true);

        cube_draw.draw()
            .set_vertex_buffer(buffer)
            .set_vertex_count(36)
            .set_primitive_type(primitive_type::triangles)
            .set_material(hdr_material);
    }

    device.submit(std::move(cube_draw), false);

    auto cubemap_vert = R"(

        #version 330 core

        layout (location = 0) in vec3 aPos;

        uniform mat4 projection;
        uniform mat4 view;

        out vec3 localPos;

        void main()
        {
            localPos = aPos;

            mat4 rotView = mat4(mat3(view)); // remove translation from the view matrix
            vec4 clipPos = projection * rotView * vec4(localPos, 1.0);

            gl_Position = clipPos.xyww;
        }
    )";

    auto cubemap_frag = R"(

        #version 330 core

        out vec4 FragColor;

        in vec3 localPos;
          
        uniform samplerCube environmentMap;
          
        void main()
        {
            vec3 envColor = texture(environmentMap, localPos).rgb;
            
            envColor = envColor / (envColor + vec3(1.0));
            envColor = pow(envColor, vec3(1.0/2.2)); 
          
            FragColor = vec4(envColor, 1.0);
        }
    )";

    auto cubemap_material = material::builder{device, shaders}
                                .set_vertex_shader(cubemap_vert)
                                .set_fragment_shader(cubemap_frag)
                                .add_uniform("projection", parameter_type::mat4)
                                .add_uniform("view", parameter_type::mat4)
                                .add_uniform("environmentMap", cubemap)
                                .set_culling_enabled(false)
                                .build();

    return model(mesh(primitive(buffer, 36, std::move(cubemap_material))));
}

class app final : public application
{
    camera camera_;

    asset_importer<model> model_importer_;

    rectangle viewport_ = {0, 0, 1280, 720};

    model model_;

    model cube_;

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
          cube_(make_hdr_environment_map(
              graphics_, app::data_path() / "Textures" / "siggraph.hdr")),
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

        light_.direction = normalize(light_.direction);

        graphics_.submit_and_swap(imgui_.draw());
    }

    void draw(const game_time delta_time) override
    {
        command_list scene_draw;

        scene_draw.viewport().set_rectangle(viewport_);

        scene_draw.scissor().set_rectangle(viewport_);

        scene_draw.clear()
            .set_color(color::antique_white())
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

        for (auto& mesh : cube_)
        {
            for (auto& primitive : mesh)
            {
                auto& material = primitive.get_material();

                material["view"] = camera_.get_view();
                material["projection"] = camera_.get_projection();

                auto& buffer = scene_draw.make_command_buffer();

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
