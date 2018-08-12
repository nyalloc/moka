#pragma once

#include <memory>
#include <asset_importer/filesystem.hpp>

namespace moka
{
    class asset_database
    {
        class impl;
        std::unique_ptr<impl> impl_;
    public:
        asset_database();
        ~asset_database();
    };
}
