#pragma once

#include <memory>
#include <asset_importer/filesystem.hpp>

namespace moka
{
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
        texutre_type type;
        texture_shape shape;
        teture_filtering filtering;
        edge_value_sampling sampling;
        bool generate_mipmaps = true;
    };

    class asset_database
    {
        class impl;
        std::unique_ptr<impl> impl_;
    public:
        asset_database();
        ~asset_database();

        texture_import_settings get_import_settings(const fs::path& path) const;
    };
}
