#include <asset_importer/asset_importer.hpp>
#include <asset_importer/mesh_importer/mesh_importer.hpp>

namespace moka
{
    asset_importer::asset_importer(const fs::path& path)
        : path_(path)
    {
        register_loader<texture_data>();
    }
}
