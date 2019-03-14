#pragma once

#include <application/application.hpp>
#include <application/open_file.hpp>
#include <asset_importer/model_importer.hpp>
#include <filesystem>
#include <graphics/camera/camera.hpp>
#include <graphics/color.hpp>
#include <graphics/device/graphics_device.hpp>
#include <graphics/material/material_builder.hpp>
#include <graphics/model.hpp>

namespace moka
{
    class pbr_util
    {
        graphics_device& device;

        vertex_buffer cube_buffer;

        vertex_buffer make_cube_buffer(buffer_usage use = buffer_usage::static_draw);

    public:
        pbr_util(graphics_device& device);

        model make_skybox(texture cubemap);

        texture build_empty_hdr_cubemap(int size, bool set_mipmaps = false);

        texture make_hdr_environment_map(const std::filesystem::path& texture_path, int environment_size = 1024);

        texture make_irradiance_environment_map(texture hdr_environment_map);

        texture make_specular_environment_map(texture hdr_environment_map);

        texture make_brdf_integration_map();
    };
} // namespace moka
