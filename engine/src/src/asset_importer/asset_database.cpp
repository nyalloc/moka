#include <asset_importer/asset_database.hpp>
#include <asset_importer/filesystem.hpp>
#include <nlohmann/json.hpp>

namespace moka
{
    class asset_database::impl
    {
        nlohmann::json json_;

    public:
        impl() = default;

        ~impl() = default;

        texture_import_settings get_import_settings(const fs::path& path) const
        {
            auto value = json_[""];
            return {};
        }
    };

    asset_database::asset_database()
        : impl_(std::make_unique<impl>())
    {
    }

    asset_database::~asset_database() = default;

    texture_import_settings asset_database::get_import_settings(const fs::path& path) const
    {
        return impl_->get_import_settings(path);
    }
}
