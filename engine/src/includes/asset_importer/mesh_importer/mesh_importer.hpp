#pragma once

#include <maths/colour.hpp>
#include <asset_importer/texture_importer/texture_importer.hpp>

namespace moka
{
    struct mesh_import_settings
    {
        color color_key;
        bool color_key_enabled = false;
        bool generate_tangent_frames = false;
        bool premultiply_vertex_colors = false;
        bool swap_winding_order = false;
        texture_import_settings texture_import_settings;
        vector3 scale{ 1, 1, 1 };
        vector3 orientation{ 0, 0, 0 };
    };
}
