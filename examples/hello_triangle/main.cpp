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
#include <filesystem>

#include "graphics/color.hpp"

using namespace moka;

class triangle_application final : public application
{
    float vertices_[18] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f};

    const char* vertex_source_ = R"(
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec4 color0;
        out vec3 out_color0;
        void main()
        {
            gl_Position = vec4(position, 1.0);
            out_color0 = color0.xyz;
        }
    )";


    const char* fragment_source_ = R"(
        out vec4 FragColor;
        in vec3 out_color0;
        void main()
        {
            FragColor = vec4(out_color0, 1.0f);
        }
    )";

    vertex_buffer_handle vertex_buffer_;
    index_buffer_handle index_buffer_;
    material_handle material_;

public:
    explicit triangle_application(const app_settings& app_settings)
        : application(app_settings)
    {
        vertex_layout layout = {
            {0, attribute_type::float32, 2, false, 6 * sizeof(float), 0},
            {1, attribute_type::float32, 3, false, 6 * sizeof(float), 3 * sizeof(float)}};

        vertex_buffer_ = graphics_.make_vertex_buffer(
            vertices_, sizeof vertices_, std::move(layout), buffer_usage::static_draw);

        material_ = graphics_.build_material()
                        .add_vertex_shader(vertex_source_)
                        .add_fragment_shader(fragment_source_)
                        .build();
    }

    ~triangle_application()
    {
        // todo: destroy graphics resources
        timer_.stop();
    }

    void draw(const game_time delta_time) override
    {
        command_list list;

        list.clear()
            .set_color(color::cornflower_blue())
            .set_clear_color(true)
            .set_clear_depth(true);

        list.draw()
            .set_vertex_buffer(vertex_buffer_)
            .set_material(material_)
            .set_vertex_count(3);

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
    app_settings settings{};
    settings.window.resolution = {1600, 900};
    settings.window.fullscreen = false;

    return triangle_application{settings}.run();
}
