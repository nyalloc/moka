
#include <application/application.hpp>
#include <glm/gtx/transform.hpp>
#include <graphics/color.hpp>

using namespace moka;

class texture_application : public application
{
    float vertices_[32] = {0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f,  1.0f, 0.5f, -0.5f, 0.0f,
                           0.0f, 1.0f, 0.0f,  1.0f, 0.0f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  1.0f,
                           0.0f, 0.0f, -0.5f, 0.5f, 0.0f, 1.0f,  1.0f,  0.0f, 0.0f, 1.0f};

    uint32_t indices_[6] = {0, 1, 3, 1, 2, 3};

    vertex_layout vertex_layout_ = {
        {0, attribute_type::float32, 3, false, 8 * sizeof(float), 0},
        {1, attribute_type::float32, 3, false, 8 * sizeof(float), 3 * sizeof(float)},
        {2, attribute_type::float32, 2, false, 8 * sizeof(float), 6 * sizeof(float)}};

    const char* vertex_source_ =
        "    #version 330 core                                  \n"
        "    layout (location = 0)  in vec3 position;           \n"
        "    layout (location = 1)  in vec4 color0;             \n"
        "    layout (location = 2)  in vec2 tex_coord0;         \n"
        "    out vec2 tex_coord;                                \n"
        "    uniform mat4 transform;                            \n"
        "    void main()                                        \n"
        "    {                                                  \n"
        "        gl_Position = transform * vec4(position, 1.0); \n"
        "        tex_coord = tex_coord0;                        \n"
        "    }                                                  \0";

    const char* fragment_source_ =
        "    #version 330 core                                      \n"
        "    out vec4 FragColor;                                    \n"
        "    in vec2 tex_coord;                                     \n"
        "    uniform sampler2D tile_texture;                        \n"
        "    uniform sampler2D test_texture;                        \n"
        "    void main()                                            \n"
        "    {                                                      \n"
        "        vec4 tex1 = texture(tile_texture, tex_coord).rgba;	\n"
        "        vec4 tex2 = texture(test_texture, tex_coord).rgba;	\n"
        "                                                           \n"
        "        FragColor = mix(tex1, tex2, tex2.a);               \n"
        "    }                                                      \0";

    vertex_buffer_handle vertex_buffer_;
    index_buffer_handle index_buffer_;

    texture_handle tile_texture_{};
    texture_handle test_texture_{};

    material_handle material_;

    glm::vec4 white_;

public:
    explicit texture_application(const app_settings& settings)
        : application(settings),
          vertex_buffer_(graphics_.make_vertex_buffer(
              vertices_, sizeof vertices_, std::move(vertex_layout_), buffer_usage::static_draw)),
          index_buffer_(graphics_.make_index_buffer(
              indices_, sizeof indices_, index_type::uint32, buffer_usage::static_draw))
    {
        int width;
        int height;
        host_format format;

        auto* data = texture_load(
            texture_application::data_path() / "Textures" / "default.png", width, height, format);

        tile_texture_ =
            graphics_.build_texture()
                .set_target(texture_target::texture_2d)
                .add_image_data(
                    image_target::texture_2d, 0, device_format::rgba, width, height, 0, format, pixel_type::uint8, data)
                .build();

        data = texture_load(texture_application::data_path() / "Textures" / "corgi.png", width, height, format);

        test_texture_ =
            graphics_.build_texture()
                .set_target(texture_target::texture_2d)
                .add_image_data(
                    image_target::texture_2d, 0, device_format::rgba, width, height, 0, format, pixel_type::uint8, data)
                .build();

        material_ = graphics_.build_material()
                        .add_material_parameter("transform", parameter_type::mat4)
                        .set_vertex_shader(vertex_source_)
                        .set_fragment_shader(fragment_source_)
                        .set_culling_enabled(false)
                        .build();

        free_texture(data);
    }

    void draw(const game_time delta_time) override
    {
        const auto current_time = seconds_elapsed();

        command_list list;

        list.clear().set_color(color::red()).set_clear_color(true).set_clear_depth(true);

        glm::mat4 trans;
        trans = glm::rotate(trans, current_time, glm::vec3(0.0f, 0.0f, 1.0f));

        list.set_material_parameters()
            .set_parameter("transform", trans)
            .set_parameter("tile_texture", tile_texture_)
            .set_parameter("test_texture", test_texture_);

        list.draw()
            .set_vertex_buffer(vertex_buffer_)
            .set_vertex_count(4)
            .set_index_buffer(index_buffer_)
            .set_index_type(index_type::uint32)
            .set_index_count(6)
            .set_material(material_);

        graphics_.submit_and_swap(std::move(list));
    }

    void update(const game_time delta_time) override
    {
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
    app_settings settings;
    settings.window_settings.resolution = {800, 800};
    return texture_application{settings}.run();
}
