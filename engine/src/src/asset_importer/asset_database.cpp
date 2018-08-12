#include <asset_importer/asset_database.hpp>
#include <nlohmann/json.hpp>

namespace moka
{
    class asset_database::impl
    {
        nlohmann::json json_;

    public:
        impl() = default;

        ~impl() = default;

    };

    asset_database::asset_database()
        : impl_(std::make_unique<impl>())
    {
    }

    asset_database::~asset_database() = default;
}
