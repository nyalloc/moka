#include <asset_importer/resource_manager.hpp>
#include <asset_importer/texture_importer/texture_importer.hpp>

namespace moka
{
    resource_manager::resource_manager(const filesystem::path& path)
        : _path(path)
    {
        register_loader<texture_2d>();
    }
}
