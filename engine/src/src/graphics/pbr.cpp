#include <graphics/default_shaders.hpp>
#include <graphics/pbr.hpp>
#include <graphics/pbr_constants.hpp>

namespace moka
{
    pbr_util::pbr_util(graphics_device& device, const std::filesystem::path& root)
        : device(device), cube_buffer(make_cube_buffer()), root(root)
    {
    }

    model pbr_util::load_model(const std::filesystem::path& gltf, const std::filesystem::path& material)
    {
        return asset_importer<model>{root, device}.load(gltf, material);
    }

    vertex_buffer pbr_util::make_cube_buffer(buffer_usage use)
    {
        const auto size = sizeof(float);

        auto cube_layout =
            vertex_layout::builder{}
                .add_attribute(0, 3, attribute_type::float32, false, 8 * size, 0)
                .add_attribute(1, 3, attribute_type::float32, false, 8 * size, 3 * size)
                .add_attribute(2, 2, attribute_type::float32, false, 8 * size, 6 * size)
                .build();

        return device.make_vertex_buffer(
            constants::cube_vertices,
            sizeof constants::cube_vertices,
            std::move(cube_layout),
            buffer_usage::static_draw);
    }

    model pbr_util::make_skybox(texture cubemap)
    {
        // irradiance_ ------------------------------------------

        auto cubemap_material = device.build_material()
                                    .set_vertex_shader(shaders::shade_cubemap::vert)
                                    .set_fragment_shader(shaders::shade_cubemap::frag)
                                    .add_uniform("projection", parameter_type::mat4)
                                    .add_uniform("view", parameter_type::mat4)
                                    .add_uniform("environmentMap", cubemap)
                                    .set_culling_enabled(false)
                                    .build();

        return model(mesh(primitive(cube_buffer, 36, cubemap_material)));
    }

    texture pbr_util::build_empty_hdr_cubemap(int size, bool set_mipmaps)
    {
        auto irradiance_texture_builder = device.build_texture();

        for (auto& image_target : constants::image_targets)
        {
            irradiance_texture_builder.add_image_data(
                image_target, 0, device_format::rgb16f, size, size, 0, host_format::rgb, pixel_type::float32, nullptr);
        }

        return irradiance_texture_builder.set_target(texture_target::cubemap)
            .set_wrap_s(wrap_mode::clamp_to_edge)
            .set_wrap_t(wrap_mode::clamp_to_edge)
            .set_wrap_r(wrap_mode::clamp_to_edge)
            .set_min_filter(min_filter::linear)
            .set_mag_filter(mag_filter::linear)
            .set_mipmaps(set_mipmaps)
            .build();
    }

    texture pbr_util::make_hdr_environment_map(const std::filesystem::path& texture_path, int environment_size)
    {
        auto height = 0;
        auto width = 0;
        host_format format;

        const auto data = texture_load_hdr(root / texture_path, width, height, format);

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

        auto hdr_material = device.build_material()
                                .set_vertex_shader(shaders::make_cubemap::vert)
                                .set_fragment_shader(shaders::make_cubemap::frag)
                                .add_uniform("projection", constants::projection)
                                .add_uniform("view", parameter_type::mat4)
                                .add_uniform("map", hdr)
                                .set_culling_enabled(false)
                                .build();

        command_list hdr_map_list;

        auto hdr_texture_builder = device.build_texture();

        for (auto& image_target : constants::image_targets)
        {
            hdr_texture_builder.add_image_data(
                image_target,
                0,
                device_format::rgb16f,
                environment_size,
                environment_size,
                0,
                host_format::rgb,
                pixel_type::float32,
                nullptr);
        }

        auto hdr_cubemap =
            hdr_texture_builder.set_target(texture_target::cubemap)
                .set_wrap_s(wrap_mode::clamp_to_edge)
                .set_wrap_t(wrap_mode::clamp_to_edge)
                .set_wrap_r(wrap_mode::clamp_to_edge)
                .set_min_filter(min_filter::linear_mipmap_linear)
                .set_mag_filter(mag_filter::linear)
                .build();

        const auto hdr_frame_buffer =
            device.build_frame_buffer()
                .add_depth_attachment(frame_format::depth_component24, environment_size, environment_size)
                .build();

        hdr_map_list.viewport().set_rectangle(0, 0, environment_size, environment_size);

        hdr_map_list.frame_buffer().set_frame_buffer(hdr_frame_buffer);

        for (auto i = 0; i < 6; i++)
        {
            hdr_map_list.set_material_parameters()
                .set_material(hdr_material)
                .set_parameter("view", constants::capture_views[i]);

            auto image_target = constants::image_targets[i];

            hdr_map_list.frame_buffer_texture()
                .set_texture(hdr_cubemap)
                .set_attachment(frame_attachment::color)
                .set_target(image_target)
                .set_mip_level(0);

            hdr_map_list.clear().set_clear_color(true).set_clear_depth(true);

            hdr_map_list.draw()
                .set_vertex_buffer(cube_buffer)
                .set_vertex_count(36)
                .set_primitive_type(primitive_type::triangles)
                .set_material(hdr_material);
        }

        hdr_map_list.generate_mipmaps().set_texture(hdr_cubemap);

        device.submit(std::move(hdr_map_list), false);

        return hdr_cubemap;
    }

    texture pbr_util::make_irradiance_environment_map(texture hdr_environment_map)
    {
        auto irradiance_material =
            device.build_material()
                .set_vertex_shader(shaders::make_irradiance_map::vert)
                .set_fragment_shader(shaders::make_irradiance_map::frag)
                .add_uniform("projection", constants::projection)
                .add_uniform("view", parameter_type::mat4)
                .add_uniform("environmentMap", hdr_environment_map)
                .set_culling_enabled(false)
                .build();

        command_list irradiance_list;

        auto irradiance_size = 32;

        auto irradiance_texture_builder = device.build_texture();

        for (auto& image_target : constants::image_targets)
        {
            irradiance_texture_builder.add_image_data(
                image_target,
                0,
                device_format::rgb16f,
                irradiance_size,
                irradiance_size,
                0,
                host_format::rgb,
                pixel_type::float32,
                nullptr);
        }

        auto irradiance_cubemap =
            irradiance_texture_builder.set_target(texture_target::cubemap)
                .set_wrap_s(wrap_mode::clamp_to_edge)
                .set_wrap_t(wrap_mode::clamp_to_edge)
                .set_wrap_r(wrap_mode::clamp_to_edge)
                .set_min_filter(min_filter::linear)
                .set_mag_filter(mag_filter::linear)
                .build();

        const auto irradiance_frame_buffer =
            device.build_frame_buffer()
                .add_depth_attachment(frame_format::depth_component24, irradiance_size, irradiance_size)
                .build();

        irradiance_list.viewport().set_rectangle(0, 0, irradiance_size, irradiance_size);

        irradiance_list.frame_buffer().set_frame_buffer(irradiance_frame_buffer);

        // each draw call renders to the side of a irradiance_
        for (auto i = 0; i < 6; i++)
        {
            irradiance_list.set_material_parameters()
                .set_material(irradiance_material)
                .set_parameter("view", constants::capture_views[i]);

            auto image_target = constants::image_targets[i];

            irradiance_list.frame_buffer_texture()
                .set_texture(irradiance_cubemap)
                .set_attachment(frame_attachment::color)
                .set_target(image_target)
                .set_mip_level(0);

            irradiance_list.clear().set_clear_color(true).set_clear_depth(true);

            irradiance_list.draw()
                .set_vertex_buffer(cube_buffer)
                .set_vertex_count(36)
                .set_primitive_type(primitive_type::triangles)
                .set_material(irradiance_material);
        }

        device.submit(std::move(irradiance_list), false);

        return irradiance_cubemap;
    }

    texture pbr_util::make_specular_environment_map(texture hdr_environment_map)
    {
        command_list prefilter_list;

        auto prefilter_size = 256;

        auto prefilter_texture_builder = device.build_texture();

        for (auto& image_target : constants::image_targets)
        {
            prefilter_texture_builder.add_image_data(
                image_target,
                0,
                device_format::rgb16f,
                prefilter_size,
                prefilter_size,
                0,
                host_format::rgb,
                pixel_type::float32,
                nullptr);
        }

        auto prefilter_cubemap =
            prefilter_texture_builder.set_target(texture_target::cubemap)
                .set_wrap_s(wrap_mode::clamp_to_edge)
                .set_wrap_t(wrap_mode::clamp_to_edge)
                .set_wrap_r(wrap_mode::clamp_to_edge)
                .set_min_filter(min_filter::linear_mipmap_linear)
                .set_mag_filter(mag_filter::linear)
                .set_mipmaps(true)
                .build();

        auto prefilter_material =
            device.build_material()
                .set_vertex_shader(shaders::make_specular_map::vert)
                .set_fragment_shader(shaders::make_specular_map::frag)
                .add_uniform("roughness", parameter_type::float32)
                .add_uniform("environmentMap", hdr_environment_map)
                .add_uniform("projection", constants::projection)
                .add_uniform("view", parameter_type::mat4)
                .set_culling_enabled(false)
                .build();

        const size_t max_mip_levels = 9;
        for (size_t mip = 0; mip < max_mip_levels; ++mip)
        {
            const auto mip_width = size_t(prefilter_size * std::pow(0.5, mip));
            const auto mip_height = size_t(prefilter_size * std::pow(0.5, mip));

            const auto prefilter_frame_buffer =
                device.build_frame_buffer()
                    .add_depth_attachment(frame_format::depth_component24, mip_width, mip_height)
                    .build();

            prefilter_list.frame_buffer().set_frame_buffer(prefilter_frame_buffer);

            prefilter_list.viewport().set_rectangle(0, 0, mip_width, mip_height);

            auto roughness =
                static_cast<float>(mip) / static_cast<float>(max_mip_levels - 1);

            prefilter_list.set_material_parameters()
                .set_material(prefilter_material)
                .set_parameter("roughness", roughness);

            for (unsigned int i = 0; i < 6; ++i)
            {
                prefilter_list.set_material_parameters()
                    .set_material(prefilter_material)
                    .set_parameter("view", constants::capture_views[i]);

                prefilter_list.frame_buffer_texture()
                    .set_attachment(frame_attachment::color)
                    .set_target(constants::image_targets[i])
                    .set_texture(prefilter_cubemap)
                    .set_mip_level(mip);

                prefilter_list.clear().set_clear_color(true).set_clear_depth(true);

                prefilter_list.draw()
                    .set_vertex_buffer(cube_buffer)
                    .set_vertex_count(36)
                    .set_primitive_type(primitive_type::triangles)
                    .set_material(prefilter_material);
            }
        }

        device.submit(std::move(prefilter_list), false);

        return prefilter_cubemap;
    }

    texture pbr_util::make_brdf_integration_map()
    {
        // brdf look up texture --------------------------------

        auto brdf_material = device.build_material()
                                 .set_vertex_shader(shaders::make_brdf_map::vert)
                                 .set_fragment_shader(shaders::make_brdf_map::frag)
                                 .build();

        command_list brdf_list;

        // clang-format on

        auto quad_layout =
            vertex_layout::builder{}
                .add_attribute(0, 3, attribute_type::float32, false, 5 * sizeof(float), 0)
                .add_attribute(
                    1, 2, attribute_type::float32, false, 5 * sizeof(float), 3 * sizeof(float))
                .build();

        const auto quad_buffer = device.make_vertex_buffer(
            constants::quad_vertices,
            sizeof constants::quad_vertices,
            std::move(quad_layout),
            buffer_usage::static_draw);

        auto brdf_size = 512;

        // allocate space for the lookup texture
        auto brdf_image = device.build_texture()
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
            device.build_frame_buffer()
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
            .set_vertex_buffer(quad_buffer)
            .set_vertex_count(4)
            .set_primitive_type(primitive_type::triangle_strip)
            .set_material(brdf_material);

        device.submit(std::move(brdf_list), false);

        return brdf_image;
    }
} // namespace moka
