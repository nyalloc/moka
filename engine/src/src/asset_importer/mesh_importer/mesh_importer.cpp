#include <asset_importer/mesh_importer/mesh_importer.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <future>

namespace moka
{
    basic_importer<texture_data>::basic_importer()
    {
    }

    texture_data basic_importer<texture_data>::load_asset(const fs::path& path) const
    {
        texture_data data;
        int component_count;
        data.data = stbi_load(path.string().c_str(), &data.size[0], &data.size[1], &component_count, STBI_default);

        if (component_count == 3)
        {
            data.format = texture_format::rgb;
        }
        else if (component_count == 4)
        {
            data.format = texture_format::rgba;
        }

        return data;
    }

    void basic_importer<texture_data>::load_asset_async(const fs::path& path, std::function<void(texture_data)> handler) const
    {
        std::async(std::launch::async, [this, &path, &handler]()
        {
            handler(load_asset(path));
        });
    }
}
