#include <asset_importer/texture_importer/texture_importer.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <future>

namespace moka
{
    asset_importer<texture_2d>::asset_importer()
    {
    }

    asset_importer<texture_2d>::~asset_importer()
    {

    }

    size_t asset_importer<texture_2d>::size() const
    { 
        return _loaded_textures.size();
    }

    texture_2d asset_importer<texture_2d>::load_asset(const filesystem::path& path)
    {
        const auto iter = _loaded_textures.find(path.string());

        // check if texture already exists
        if(iter != _loaded_textures.end())
        {
            return texture_2d{ iter->second.lock() };
        }

        // custom deleter. remove resource from the map when reference count reaches 0
        std::shared_ptr<texture_data> data(new texture_data(), [this, path](texture_data *dead)
        {
            stbi_image_free(dead->data);
            delete dead;
            _loaded_textures.erase(path.string());
        });

        int component_count;
        data->data = stbi_load(path.string().c_str(), &data->size[0], &data->size[1], &component_count, STBI_default);

        if(data->data == nullptr)
        {
            throw std::runtime_error("Error: texture data was nullptr.");
        }

        if (component_count == 3)
        {
            data->format = texture_format::rgb;
        }
        else if (component_count == 4)
        {
            data->format = texture_format::rgba;
        }

        _loaded_textures.emplace(path.string(), data);

        return texture_2d{ data };
    }
}
