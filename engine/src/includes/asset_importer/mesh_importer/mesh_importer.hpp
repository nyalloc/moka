#pragma once

#include <asset_importer/filesystem.hpp>
#include <asset_importer/basic_importer.hpp>
#include "maths/vector2.hpp"
#include <functional>

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

    template<>
    class basic_importer<texture_data>
    {
    public:
        basic_importer();

        texture_data load_asset(const fs::path& path) const;

        void load_asset_async(const fs::path& path, std::function<void(texture_data)> handler) const;
    };
}
