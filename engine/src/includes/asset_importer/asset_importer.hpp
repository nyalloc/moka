#pragma once

#include <asset_importer/filesystem.hpp>
#include <asset_importer/basic_importer.hpp>
#include <unordered_map>
#include <any>
#include <typeindex>
#include <functional>
#include <asset_importer/asset_database.hpp>

namespace moka
{
    class asset_importer
    {
        asset_database database_;
        std::unordered_map<std::type_index, std::any> asset_loaders_;
        fs::path path_;
    public:
        explicit asset_importer(const fs::path& path);

        template<typename T>
        void register_loader();

        template<typename T>
        void load_asset_async(const fs::path& path, std::function<void(T)> handler);

        template<typename T>
        T load_asset(const fs::path& path);
    };

    template <typename T>
    void asset_importer::register_loader()
    {
        asset_loaders_[typeid(T)] = basic_importer<T>{};
    }

    template <typename T>
    void asset_importer::load_asset_async(const fs::path& path, std::function<void(T)> handler)
    {
        auto loader = asset_loaders_.at(typeid(T));

        auto loader_cast = std::any_cast<basic_importer<T>>(loader);

        loader_cast.load_asset_async(path_ / path, handler);
    }

    template <typename T>
    T asset_importer::load_asset(const fs::path& path)
    {
        auto loader = asset_loaders_.at(typeid(T));

        auto loader_cast = std::any_cast<basic_importer<T>>(loader);

        return loader_cast.load_asset(path_ / path);
    }
}
