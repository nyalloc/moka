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

#include <application/application.hpp>
#include <filesystem>
#include <functional>
#include <graphics/device/graphics_device.hpp>
#include <graphics/model.hpp>

namespace moka
{
    using draw_callback = std::function<void(command_list&)>;

    class pbr_util
    {
        graphics_device& device_;

        vertex_buffer_handle cube_buffer_;

        vertex_buffer_handle quad_buffer_;

        vertex_buffer_handle make_cube_buffer(buffer_usage use = buffer_usage::static_draw) const;

        vertex_buffer_handle make_quad_buffer(buffer_usage use = buffer_usage::static_draw) const;

        std::filesystem::path root_;

        /**
         * \brief Create an empty HDR cubemap texture.
         * \param size The size of the cubemap faces, where size = width = height.
         * \param set_mipmaps Should this cubemap build mipmaps?
         * \return The empty HDR cubemap.
         */
        texture_handle make_empty_hdr_cubemap(uint32_t size, min_filter filter, bool set_mipmaps = false) const;

        void draw_cubemap_faces(command_list& list, uint32_t mip_level, texture_handle cubemap, material_handle material) const;

        /**
         * \brief Create an empty HDR cubemap texture.
         * \param size The size of the cubemap faces, where size = width = height.
         * \param set_mipmaps Should this cubemap build mipmaps?
         * \return The empty HDR cubemap.
         */
        void draw_to_cubemap(
            uint32_t size,
            uint32_t mip_level,
            texture_handle cubemap,
            material_handle material,
            draw_callback&& pre = {},
            draw_callback&& post = {}) const;

    public:
        /**
         * \brief Create a PBR utility.
         * \param device The graphics device to use when creating PBR resources.
         * \param root The root resource folder.
         */
        pbr_util(graphics_device& device, const std::filesystem::path& root);

        /**
         * \brief Load a model.
         * \param gltf Relative path to the glTF asset.
         * \param material Relative path to the material file.
         * \return The imported model asset.
         */
        model load_model(const std::filesystem::path& gltf, const std::filesystem::path& material) const;

        /**
         * \brief Create a skybox model.
         * \param cubemap The cubemap texture.
         * \return The skybox model.
         */
        model make_skybox(texture_handle cubemap) const;

        /**
         * \brief Load an equirectangular HDRI envrironment map.
         * \param texture_path Relative path to the HDR texture.
         * \return The HDR texture.
         */
        texture_handle import_equirectangular_map(const std::filesystem::path& texture_path) const;

        /**
         * \brief Create an HDR environment map from an equirectangular HDR
         * texture. \param equirectangular_map The equirectangular map you want
         * to render to a cubemap. \param environment_size The size of the
         * cubemap to render. \return The cubemap texture.
         */
        texture_handle equirectangular_to_cubemap(
            texture_handle equirectangular_map, uint32_t environment_size = 1024) const;

        /**
         * \brief Create an irradiance environment map.
         * \param hdr_environment_map The source HDR cubemap to blur + turn into
         * an irradiance map. \return The irradiance environment map.
         */
        texture_handle make_irradiance_environment_map(texture_handle hdr_environment_map) const;

        /**
         * \brief Create a specular environment map.
         * \param hdr_environment_map The source HDR cubemap to blur + turn into
         * an specular map. \return The specular environment map.
         */
        texture_handle make_specular_environment_map(texture_handle hdr_environment_map) const;

        /**
         * \brief Create a BRDF integration map.
         * \return The BRDF integration map.
         */
        texture_handle make_brdf_integration_map() const;
    };
} // namespace moka
