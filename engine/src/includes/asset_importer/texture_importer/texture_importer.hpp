#pragma once

#include <functional>
#include <maths/vector2.hpp>
#include <asset_importer/filesystem.hpp>
#include <asset_importer/asset_importer.hpp>
#include "maths/colour.hpp"
#include <atomic>

namespace moka
{
    enum class texture_format
    {
        rgb,
        rgba
    };

    struct texture_data
    {
        texture_format format{};
        point2 size{ 0, 0 };
        void* data;
    };

    enum class texutre_type
    {
        default_settings,
        normal_map,
    };

    enum class texture_shape
    {
        tex2d, tex3d
    };

    enum class teture_filtering
    {
        nearest,
        linear,
    };

    enum class edge_value_sampling
    {
        repeat,
        mirrored_repeat,
        clamp_to_edge,
        clamp_to_border,
        mirror_clamp_to_edge
    };

    struct texture_import_settings
    {
        color color_key;
        bool color_key_enabled = false;
        bool generate_mipmaps = false;
        bool make_square = false;
        bool premultiply_alpha = false;
        bool resize_to_power_of_two = false;
        teture_filtering filtering;
        edge_value_sampling sampling;
    };

    class texture_2d
    {
        std::shared_ptr<texture_data> data;
    public:
        explicit texture_2d(const std::shared_ptr<texture_data> data)
            : data(data)
        {}

        texture_format format() const
        {
            return data->format;
        }

        const point2& size() const
        {
            return data->size;
        }
    };

    template<>
    class asset_importer<texture_2d> : public base_importer
    {
        std::unordered_map<std::string, std::weak_ptr<texture_data>> _loaded_textures;
    public:
        asset_importer();

        ~asset_importer();

        size_t size() const;

        texture_2d load_asset(const filesystem::path& path);
    };
}
