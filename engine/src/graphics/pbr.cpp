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

References:

    Ericson, C. (2008). Order your graphics draw calls around!.
    [online] realtimecollisiondetection.net � the blog.
    Available at: http://realtimecollisiondetection.net/blog/?p=86 [Accessed 21 Mar. 2019].

    Pranckevicius, A. (2014). Rough sorting by depth � Aras' website. [online] Aras' website.
    Available at: http://aras-p.info/blog/2014/01/16/rough-sorting-by-depth/ [Accessed 22 Mar. 2019].

===========================================================================
*/
#include <asset_importer/model_importer.hpp>
#include <graphics/default_shaders.hpp>
#include <graphics/pbr.hpp>
#include <graphics/pbr_constants.hpp>

namespace moka
{
    pbr_util::pbr_util(graphics_device& device, const std::filesystem::path& root)
        : device_(device),
          cube_buffer_(make_cube_buffer()),
          quad_buffer_(make_quad_buffer()),
          root_(root)
    {
    }

    model pbr_util::load_model(const std::filesystem::path& gltf, const std::filesystem::path& material) const
    {
        return asset_importer<model>{root_, device_}.load(gltf, material);
    }

    vertex_buffer_handle pbr_util::make_quad_buffer(buffer_usage use) const
    {
        const auto size = sizeof(float);

        auto quad_layout =
            vertex_layout::builder{}
                .add_attribute(0, 3, attribute_type::float32, false, 5 * size, 0)
                .add_attribute(1, 2, attribute_type::float32, false, 5 * size, 3 * size)
                .build();

        return device_.make_vertex_buffer(
            constants::quad_vertices,
            sizeof constants::quad_vertices,
            std::move(quad_layout),
            buffer_usage::static_draw);
    }

    vertex_buffer_handle pbr_util::make_cube_buffer(buffer_usage use) const
    {
        const auto size = sizeof(float);

        auto cube_layout =
            vertex_layout::builder{}
                .add_attribute(0, 3, attribute_type::float32, false, 8 * size, 0)
                .add_attribute(1, 3, attribute_type::float32, false, 8 * size, 3 * size)
                .add_attribute(2, 2, attribute_type::float32, false, 8 * size, 6 * size)
                .build();

        return device_.make_vertex_buffer(
            constants::cube_vertices,
            sizeof constants::cube_vertices,
            std::move(cube_layout),
            buffer_usage::static_draw);
    }

    model pbr_util::make_skybox(const texture_handle cubemap) const
    {
        // irradiance_ ------------------------------------------

        const auto cubemap_material =
            device_.build_material()
                .add_vertex_shader(shaders::shade_cubemap::vert)
                .add_fragment_shader(shaders::shade_cubemap::frag)
                .add_material_parameter("projection", parameter_type::mat4)
                .add_material_parameter("view", parameter_type::mat4)
                .add_material_parameter("environment_map", cubemap)
                .set_culling_enabled(false)
                .build();

        return model(mesh(primitive(cube_buffer_, 36, cubemap_material)));
    }

    texture_handle pbr_util::make_empty_hdr_cubemap(const uint32_t size, min_filter filter, bool set_mipmaps) const
    {
        auto irradiance_texture_builder = device_.build_texture();

        for (const auto& image_target : constants::image_targets)
        {
            irradiance_texture_builder.add_image_data(
                image_target, 0, device_format::rgb16f, size, size, 0, host_format::rgb, pixel_type::float32, nullptr);
        }

        return irradiance_texture_builder.set_target(texture_target::cubemap)
            .set_wrap_s(wrap_mode::clamp_to_edge)
            .set_wrap_t(wrap_mode::clamp_to_edge)
            .set_wrap_r(wrap_mode::clamp_to_edge)
            .set_min_filter(filter)
            .set_mag_filter(mag_filter::linear)
            .set_mipmaps(set_mipmaps)
            .build();
    }

    void pbr_util::draw_cubemap_faces(
        command_list& list, uint32_t mip_level, texture_handle cubemap, material_handle material) const
    {
        for (auto i = 0; i < 6; i++)
        {
            list.set_material_parameters().set_material(material).set_parameter(
                "view", constants::capture_views[i]);

            const auto image_target = constants::image_targets[i];

            list.frame_buffer_texture()
                .set_texture(cubemap)
                .set_attachment(frame_attachment::color)
                .set_target(image_target)
                .set_mip_level(mip_level);

            list.clear().set_clear_color(true).set_clear_depth(true);

            list.draw()
                .set_vertex_buffer(cube_buffer_)
                .set_vertex_count(36)
                .set_primitive_type(primitive_type::triangles)
                .set_material(material);
        }
    }

    void pbr_util::draw_to_cubemap(
        uint32_t size,
        uint32_t mip_level,
        texture_handle cubemap,
        material_handle material,
        draw_callback&& pre,
        draw_callback&& post) const
    {
        command_list list;

        const auto hdr_frame_buffer =
            device_.build_frame_buffer()
                .add_depth_attachment(frame_format::depth_component24, size, size)
                .build();

        list.viewport().set_rectangle(0, 0, size, size);
        list.frame_buffer().set_frame_buffer(hdr_frame_buffer);

        if (pre)
        {
            pre(list);
        }

        draw_cubemap_faces(list, mip_level, cubemap, material);

        if (post)
        {
            post(list);
        }

        list.frame_buffer().set_frame_buffer({0});
        device_.submit(std::move(list), false);
        device_.destroy(hdr_frame_buffer);
    }

    texture_handle pbr_util::import_equirectangular_map(const std::filesystem::path& texture_path) const
    {
        auto height = 0;
        auto width = 0;
        host_format format;

        auto* data = texture_load_hdr(root_ / texture_path, width, height, format);

        return device_.build_texture()
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
    }

    texture_handle pbr_util::equirectangular_to_cubemap(
        texture_handle equirectangular_map, const uint32_t environment_size) const
    {
        const auto hdr_material =
            device_.build_material()
                .add_vertex_shader(shaders::equirectangular_to_cube::vert)
                .add_fragment_shader(shaders::equirectangular_to_cube::frag)
                .add_material_parameter("projection", constants::projection)
                .add_material_parameter("view", glm::mat4{})
                .add_material_parameter("map", equirectangular_map)
                .set_culling_enabled(false)
                .build();

        const auto hdr_cubemap = make_empty_hdr_cubemap(
            environment_size, min_filter::linear_mipmap_linear, false);

        draw_to_cubemap(
            environment_size,
            0, // mip level being drawn to
            hdr_cubemap,
            hdr_material,
            {}, // submit optional pre-draw commands here
            [&](command_list& list) {
                // we want to generate mips after drawing
                list.generate_mipmaps().set_texture(hdr_cubemap);
            });

        return hdr_cubemap;
    }

    texture_handle pbr_util::make_irradiance_environment_map(texture_handle hdr_environment_map) const
    {
        const auto irradiance_material =
            device_.build_material()
                .add_vertex_shader(shaders::make_irradiance_map::vert)
                .add_fragment_shader(shaders::make_irradiance_map::frag)
                .add_material_parameter("projection", constants::projection)
                .add_material_parameter("view", glm::mat4{})
                .add_material_parameter("environment_map", hdr_environment_map)
                .set_culling_enabled(false)
                .build();

        const auto irradiance_size = 32;

        const auto irradiance_cubemap =
            make_empty_hdr_cubemap(irradiance_size, min_filter::linear, false);

        draw_to_cubemap(irradiance_size, 0, irradiance_cubemap, irradiance_material);

        return irradiance_cubemap;
    }

    texture_handle pbr_util::make_specular_environment_map(texture_handle hdr_environment_map) const
    {
        const auto prefilter_size = 256;

        const auto prefilter_cubemap = make_empty_hdr_cubemap(
            prefilter_size, min_filter::linear_mipmap_linear, true);

        const auto prefilter_material =
            device_.build_material()
                .add_vertex_shader(shaders::make_specular_map::vert)
                .add_fragment_shader(shaders::make_specular_map::frag)
                .add_material_parameter("roughness", 0.0f)
                .add_material_parameter("environment_map", hdr_environment_map)
                .add_material_parameter("projection", constants::projection)
                .add_material_parameter("view", glm::mat4{})
                .set_culling_enabled(false)
                .build();

        const uint32_t max_mip_levels = 5;
        for (uint32_t mip = 0; mip < max_mip_levels; ++mip)
        {
            const auto mip_size = static_cast<uint32_t>(prefilter_size * std::pow(0.5, mip));

            draw_to_cubemap(
                mip_size, mip, prefilter_cubemap, prefilter_material, [&](command_list& list) {
                    const auto roughness = static_cast<float>(mip) /
                                           static_cast<float>(max_mip_levels - 1);

                    list.set_material_parameters()
                        .set_material(prefilter_material)
                        .set_parameter("roughness", roughness);
                });
        }

        return prefilter_cubemap;
    }

    texture_handle pbr_util::make_brdf_integration_map() const
    {
        const auto brdf_material =
            device_.build_material()
                .add_vertex_shader(shaders::make_brdf_map::vert)
                .add_fragment_shader(shaders::make_brdf_map::frag)
                .build();

        command_list brdf_list;

        const auto brdf_size = 512;

        // allocate space for the lookup texture
        const auto brdf_image = device_.build_texture()
                                    .add_image_data(
                                        image_target::texture_2d,
                                        0,
                                        device_format::rg16f,
                                        brdf_size,
                                        brdf_size,
                                        0,
                                        host_format::rg,
                                        pixel_type::float32,
                                        nullptr)
                                    .set_target(texture_target::texture_2d)
                                    .set_wrap_s(wrap_mode::clamp_to_edge)
                                    .set_wrap_t(wrap_mode::clamp_to_edge)
                                    .set_min_filter(min_filter::linear)
                                    .set_mag_filter(mag_filter::linear)
                                    .build();

        // create a framebuffer to render to
        const auto brdf_frame_buffer =
            device_.build_frame_buffer()
                .add_depth_attachment(frame_format::depth_component24, brdf_size, brdf_size)
                .build();

        brdf_list.frame_buffer().set_frame_buffer(brdf_frame_buffer);

        brdf_list.viewport().set_rectangle(0, 0, brdf_size, brdf_size);

        brdf_list.frame_buffer_texture()
            .set_texture(brdf_image)
            .set_attachment(frame_attachment::color)
            .set_target(image_target::texture_2d)
            .set_mip_level(0);

        brdf_list.clear().set_clear_color(true).set_clear_depth(true);

        brdf_list.draw()
            .set_vertex_buffer(quad_buffer_)
            .set_vertex_count(4)
            .set_primitive_type(primitive_type::triangle_strip)
            .set_material(brdf_material);

        brdf_list.frame_buffer().set_frame_buffer({0});

        device_.submit(std::move(brdf_list), false);

        device_.destroy(brdf_frame_buffer);

        return brdf_image;
    }
} // namespace moka
