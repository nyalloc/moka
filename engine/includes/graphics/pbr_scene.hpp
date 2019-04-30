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
#pragma once

#include "../deps/nlohmann/json.hpp"
#include <application/application.hpp>
#include <filesystem>
#include <fstream>
#include <graphics/camera/basic_camera.hpp>
#include <graphics/color.hpp>
#include <graphics/device/graphics_device.hpp>
#include <graphics/model.hpp>
#include <graphics/pbr.hpp>

namespace moka
{
    struct directional_light
    {
        glm::vec3 direction = {0.0f, -0.8f, -0.5f};
        glm::vec3 diffuse = color::white();
        glm::vec3 ambient = color::mine_shaft();
    };

    class pbr_scene
    {
        texture_handle hdr_{};

        texture_handle irradiance_{};

        texture_handle brdf_{};

        texture_handle prefiltered_{};

        model model_;

        model cube_;

        graphics_device& device_;

        static uint32_t depth_to_bits(const float depth)
        {
            // http://aras-p.info/blog/2014/01/16/rough-sorting-by-depth/
            // use type punning to reinterpret the data of a float as an unsigned
            union {
                float f = 0.0f;
                uint32_t i;
            } data;

            data.f = depth;

            // take highest 10 bits
            return data.i >> 22;
        }

        static sort_key generate_sort_key(const float depth, const uint16_t material_id, const alpha_mode alpha)
        {
            // http://realtimecollisiondetection.net/blog/?p=86
            // sort by alpha, then by depth, then by material
            return static_cast<sort_key>(material_id) |
                   static_cast<sort_key>(depth_to_bits(depth)) << 16 |
                   static_cast<sort_key>(alpha == alpha_mode::blend) << 48;
        }

    public:
        // need to expose these to bind them to imgui - might re-evaluate this later!
        glm::vec4 color = color::burnt_sienna();
        bool draw_environment = true;
        bool use_ibl = true;
        bool use_directional_light = true;
        float gamma = 2.2f;
        float exposure = 1.0f;
        size_t active_program = 0;
        directional_light light;

        /**
         * \brief Create a new scene object.
         * \param device Graphics device object to use with the scene.
         * \param root The root resource folder where PBR assets are located.
         */
        pbr_scene(graphics_device& device, const std::filesystem::path& root)
            : device_(device)
        {
            pbr_util util(device, root);

            std::ifstream i(root / "config.json");
            nlohmann::json j;
            i >> j;

            const auto& model = j["config"]["model"].get<std::string>();
            const auto& draw_environment = j["config"]["environment"].get<std::string>();

            model_ = util.load_model(model, "Materials/pbr.material");

            hdr_ = util.equirectangular_to_cubemap(
                util.import_equirectangular_map(draw_environment));

            irradiance_ = util.make_irradiance_environment_map(hdr_);

            prefiltered_ = util.make_specular_environment_map(hdr_);

            brdf_ = util.make_brdf_integration_map();

            cube_ = util.make_skybox(hdr_);
        }

        /**
         * \brief Draw the scene.
         * \param camera The camera to draw with the scene.
         * \param viewport The viewport to draw the scene with.
         */
        void draw(const basic_camera& camera, const rectangle& viewport)
        {
            const auto& view_pos = camera.get_position();

            command_list scene_draw;

            scene_draw.viewport().set_rectangle(viewport);

            scene_draw.scissor().set_rectangle(viewport);

            scene_draw.clear().set_color(color).set_clear_color(true).set_clear_depth(true);

            for (auto& mesh : model_)
            {
                for (auto& primitive : mesh)
                {
                    const auto material = primitive.get_material();

                    auto* mat = device_.get_material_cache().get_material(material);

                    if (mat)
                    {
                        mat->set_active_program(active_program);

                        auto pos = mesh.get_transform().get_world_position();

                        const auto distance = glm::distance2(pos, view_pos);

                        const auto sort_key = generate_sort_key(
                            distance, mat->get_program().id, mat->get_alpha_mode());

                        auto& buffer = scene_draw.make_command_buffer(sort_key);

                        buffer.set_material_parameters()
                            .set_material(material)
                            .set_parameter("gamma", gamma)
                            .set_parameter("exposure", exposure)
                            .set_parameter("irradiance_map", irradiance_)
                            .set_parameter("prefilter_map", prefiltered_)
                            .set_parameter("brdf_lut", brdf_)
                            .set_parameter("model", mesh.get_transform().to_matrix())
                            .set_parameter("view", camera.get_view())
                            .set_parameter("projection", camera.get_projection())
                            .set_parameter("view_pos", view_pos)
                            .set_parameter("light.direction", light.direction)
                            .set_parameter("light.ambient", light.ambient)
                            .set_parameter("light.diffuse", light.diffuse)
                            .set_parameter("use_ibl", use_ibl)
                            .set_parameter("use_directional_light", use_directional_light);
                        primitive.draw(buffer);
                    }
                }
            }

            if (draw_environment)
            {
                for (auto& mesh : cube_)
                {
                    for (auto& primitive : mesh)
                    {
                        const auto material = primitive.get_material();

                        auto& buffer = scene_draw.make_command_buffer();

                        buffer.set_material_parameters()
                            .set_material(material)
                            .set_parameter("gamma", gamma)
                            .set_parameter("exposure", exposure)
                            .set_parameter("view", camera.get_view())
                            .set_parameter("projection", camera.get_projection());

                        primitive.draw(buffer);
                    }
                }
            }

            device_.submit(std::move(scene_draw));
        }
    };
} // namespace moka
