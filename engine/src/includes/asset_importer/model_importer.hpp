#pragma once

#include <asset_importer/asset_importer.hpp>
#include <filesystem>
#include <graphics/model.hpp>
#include <map>
#include <string>

namespace moka
{
    template <>
    class asset_importer<model>
    {
        graphics_device& device_;
        std::filesystem::path root_directory_;
        std::map<std::string, program_handle> shaders_;

    public:
        asset_importer(const std::filesystem::path& path, graphics_device& device);

        model load(const std::filesystem::path& model, const std::filesystem::path& material);
    };
} // namespace moka
