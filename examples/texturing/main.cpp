/*
===========================================================================
Moka Source Code
Copyright 2019 Stuart Adams. All rights reserved.
https://github.com/stuartdadams/moka
stuartdadams | linkedin.com/in/stuartdadams

This file is part of the Moka Real-Time Physically-Based Rendering Project.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

===========================================================================
*/

#include <application/application.hpp>
#include <glm/gtx/transform.hpp>
#include <graphics/color.hpp>

using namespace moka;

class texture_application final : public application
{
    float vertices_[32] = {0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                           0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                           -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                           -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f};

    uint32_t indices_[6] = {0, 1, 3, 1, 2, 3};

    vertex_layout vertex_layout_ = {
        {0, attribute_type::float32, 3, false, 8 * sizeof(float), 0},
        {1, attribute_type::float32, 3, false, 8 * sizeof(float), 3 * sizeof(float)},
        {2, attribute_type::float32, 2, false, 8 * sizeof(float), 6 * sizeof(float)}};

    const char* vertex_source_ = R"(
        layout (location = 0)  in vec3 position;
        layout (location = 1)  in vec4 color0;
        layout (location = 2)  in vec2 tex_coord0;
        out vec2 tex_coord;
        uniform mat4 transform;
        void main()
        {
            gl_Position = transform * vec4(position, 1.0);
            tex_coord = tex_coord0;
        }
    )";


    const char* fragment_source_ = R"(
        out vec4 FragColor;
        in vec2 tex_coord;
        uniform sampler2D tile_texture;
        uniform sampler2D test_texture;
        void main()
        {
            vec4 tex1 = texture(tile_texture, tex_coord).rgba;
            vec4 tex2 = texture(test_texture, tex_coord).rgba;
            FragColor = mix(tex1, tex2, tex2.a);
        }
    )";

    vertex_buffer_handle vertex_buffer_;
    index_buffer_handle index_buffer_;

    texture_handle tile_texture_{};
    texture_handle test_texture_{};

    material_handle material_{};

    glm::vec4 white_{};

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

        data = texture_load(
            texture_application::data_path() / "Textures" / "corgi.png", width, height, format);

        test_texture_ =
            graphics_.build_texture()
                .set_target(texture_target::texture_2d)
                .add_image_data(
                    image_target::texture_2d, 0, device_format::rgba, width, height, 0, format, pixel_type::uint8, data)
                .build();

        material_ = graphics_.build_material()
                        .add_material_parameter("transform", parameter_type::mat4)
                        .add_vertex_shader(vertex_source_)
                        .add_fragment_shader(fragment_source_)
                        .set_culling_enabled(false)
                        .build();

        free_texture(data);
    }

    void draw(const game_time delta_time) override
    {
        const auto current_time = seconds_elapsed();

        command_list list;

        list.clear().set_color(color::cornflower_blue()).set_clear_color(true).set_clear_depth(true);

        glm::mat4 trans(1.0f);
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
    settings.window.resolution = {800, 800};
    return texture_application{settings}.run();
}
