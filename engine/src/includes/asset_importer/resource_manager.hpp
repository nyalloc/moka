#pragma once

#include <asset_importer/filesystem.hpp>
#include <asset_importer/asset_importer.hpp>
#include <unordered_map>
#include <any>
#include <typeindex>
#include <functional>
#include <asset_importer/asset_database.hpp>
#include <future>

namespace moka
{
    /**
     * \brief resource manager is a flyweight factory for different resources.
              Many moka:: classes rely on shared state. resource_manager caches these shared resources.
              Resources are de-allocated automatically once they are no longer referenced.
     */
    class resource_manager
    {
        asset_database _database;

        // todo: identify how to tackle this problem without std::shared_ptr. It is very intrusive.
        // std::any is no good as std::any_cast produces a copy of the importer, invalidating all the references to the cached resources inside it
        std::unordered_map<std::type_index, std::shared_ptr<base_importer>> _asset_loaders;

        filesystem::path _path;
    public:
        explicit resource_manager(const filesystem::path& path);

        template <typename T, typename... Args>
        void register_loader(Args&&... args);

        template<typename T>
        T load_asset(const filesystem::path& path);

        template <class T>
        void load_asset_async(const filesystem::path& path, std::function<void(T)> handler);

        template <class T>
        std::future<T> load_asset_async(const filesystem::path& path);

        template <class T>
        size_t size() const;
    };

    template <typename T, typename... Args>
    void resource_manager::register_loader(Args&&... args)
    {
        _asset_loaders[typeid(T)] = std::make_shared<asset_importer<T>>(std::forward<Args>(args)...);
    }

    template <typename T>
    T resource_manager::load_asset(const filesystem::path& path)
    {
        auto loader = _asset_loaders.at(typeid(T));

        auto loader_cast = std::dynamic_pointer_cast<asset_importer<T>>(loader);

        return loader_cast->load_asset(_path / path);
    }

    template <typename T>
    void resource_manager::load_asset_async(const filesystem::path& path, std::function<void(T)> handler)
    {
        std::async(std::launch::async, [this]()
        {
            handler(load_asset<T>(path));
        });
    }

    template <typename T>
    std::future<T> resource_manager::load_asset_async(const filesystem::path& path)
    {
        return std::async(std::launch::async, [this]()
        {
            load_asset<T>(path);
        });
    }

    template <class T>
    size_t resource_manager::size() const
    {
        auto loader = _asset_loaders.at(typeid(T));

        auto loader_cast = std::dynamic_pointer_cast<asset_importer<T>>(loader);

        return loader_cast->size();
    }
}
