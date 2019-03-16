#pragma once

#include <asset_importer/asset_importer.hpp>
#include <filesystem>
#include <graphics/model.hpp>

namespace moka
{
    /**
     * \brief Asset importer for models. Currently only supports .gltf models.
     */
    template <>
    class asset_importer<model>
    {
        graphics_device& device_;
        std::filesystem::path root_directory_;

    public:
        /**
         * \brief Construct a new model asset importer.
         * \param path The asset folder that all model paths are relative to.
         * \param device Graphics device to upload asset information to.
         */
        asset_importer(const std::filesystem::path& path, graphics_device& device);

        /**
         * \brief Import a new model.
         * \param model_path The model you would like to import.
         * \param material_path The material you want to use with this model.
         * \return The new model.
         */
        model load(const std::filesystem::path& model_path, const std::filesystem::path& material_path) const;
    };
} // namespace moka
