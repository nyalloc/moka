#pragma once

#include "../deps/nlohmann/json.hpp"
#include <application/application.hpp>
#include <application/open_file.hpp>
#include <asset_importer/model_importer.hpp>
#include <filesystem>
#include <fstream>
#include <graphics/camera/basic_camera.hpp>
#include <graphics/color.hpp>
#include <graphics/device/graphics_device.hpp>
#include <graphics/material/material_builder.hpp>
#include <graphics/model.hpp>
#include <graphics/pbr.hpp>
#include <iostream>

namespace moka
{
    class scene
    {
        texture hdr_{};

        texture irradiance_{};

        texture brdf_{};

        texture prefiltered_{};

        model model_;

        model cube_;

        graphics_device& device;

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
            return static_cast<sort_key>(material_id) | static_cast<sort_key>(depth_to_bits(depth)) << 16 |
                   static_cast<sort_key>(alpha == alpha_mode::blend) << 48;
        }

    public:
        // need to expose these to bind them to imgui - might re-evaluate this later!
        glm::vec4 color_ = color::burnt_sienna();
        bool environment_ = true;
        float gamma_ = 2.2f;
        float exposure_ = 1.0f;

        scene(graphics_device& device, const std::filesystem::path& root) : device(device)
        {
            pbr_util util(device, root);

            std::ifstream i(root / "config.json");
            nlohmann::json j;
            i >> j;

            auto model = j["config"]["model"].get<std::string>();
            auto environment = j["config"]["environment"].get<std::string>();

            model_ = util.load_model(model, "Materials/pbr.material");

            hdr_ = util.make_hdr_environment_map(environment);

            irradiance_ = util.make_irradiance_environment_map(hdr_);

            prefiltered_ = util.make_specular_environment_map(hdr_);

            brdf_ = util.make_brdf_integration_map();

            cube_ = util.make_skybox(hdr_);
        }

        void draw(const basic_camera& camera_, const rectangle& viewport)
        {
            const auto& view_pos = camera_.get_position();

            command_list scene_draw;

            scene_draw.viewport().set_rectangle(viewport);

            scene_draw.scissor().set_rectangle(viewport);

            scene_draw.clear().set_color(color_).set_clear_color(true).set_clear_depth(true);

            for (auto& mesh : model_)
            {
                for (auto& primitive : mesh)
                {
                    auto material = primitive.get_material();

                    const auto distance =
                        glm::distance(mesh.get_transform().get_position(), camera_.get_position());

                    auto* mat = device.get_material_cache().get_material(material);

                    if (mat)
                    {
                        auto pos = mesh.get_transform().get_position();

                        const auto sort_key =
                            generate_sort_key(distance, mat->get_program().id, mat->get_alpha_mode());

                        auto& buffer = scene_draw.make_command_buffer(sort_key);

                        buffer.set_material_parameters()
                            .set_material(material)
                            .set_parameter("gamma", gamma_)
                            .set_parameter("exposure", exposure_)
                            .set_parameter("irradiance_map", irradiance_)
                            .set_parameter("prefilter_map", prefiltered_)
                            .set_parameter("brdf_lut", brdf_)
                            .set_parameter("model", mesh.get_transform().to_matrix())
                            .set_parameter("view", camera_.get_view())
                            .set_parameter("projection", camera_.get_projection())
                            .set_parameter("view_pos", view_pos);

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

                        auto& buffer = scene_draw.make_command_buffer();

                        buffer.set_material_parameters()
                            .set_material(material)
                            .set_parameter("gamma", gamma_)
                            .set_parameter("exposure", exposure_)
                            .set_parameter("view", camera_.get_view())
                            .set_parameter("projection", camera_.get_projection());

                        primitive.draw(buffer);
                    }
                }
            }

            device.submit(std::move(scene_draw));
        }
    };
} // namespace moka
