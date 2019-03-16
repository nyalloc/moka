#pragma once

#include <application/application.hpp>
#include <filesystem>
#include <graphics/device/graphics_device.hpp>
#include <graphics/material/material_builder.hpp>
#include <graphics/model.hpp>

namespace moka
{
    class pbr_util
    {
        graphics_device& device;

        vertex_buffer_handle cube_buffer;

        vertex_buffer_handle make_cube_buffer(buffer_usage use = buffer_usage::static_draw) const;

        std::filesystem::path root;

    public:
        pbr_util(graphics_device& device, const std::filesystem::path& root);

        model load_model(const std::filesystem::path& gltf, const std::filesystem::path& material) const;

        model make_skybox(texture_handle cubemap) const;

        texture_handle build_empty_hdr_cubemap(int size, bool set_mipmaps = false) const;

        texture_handle make_hdr_environment_map(const std::filesystem::path& texture_path, int environment_size = 1024) const;

        texture_handle make_irradiance_environment_map(texture_handle hdr_environment_map) const;

        texture_handle make_specular_environment_map(texture_handle hdr_environment_map) const;

        texture_handle make_brdf_integration_map() const;
    };
} // namespace moka
