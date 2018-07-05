
#include <asset_importer/asset_importer.hpp>
#include <asset_importer/mesh_importer/mesh_importer.hpp>
#include <application/application.hpp>
#include <asset_importer/filesystem.hpp>

int main()
{
    const auto path = moka::application::data_path() 
        / ".." / ".." / ".." / "examples" / "resources";

    moka::asset_importer importer{ path };

    moka::safe_queue<moka::texture_data> queue;

    for (size_t i = 0; i < 10; i++)
    {
        importer.load_asset_async<moka::texture_data>("tile.png", [&](moka::texture_data texture)
        {
            queue.enqueue(std::move(texture));
        });
    }

    for (size_t i = 0; i < 10; i++)
    {
        auto texture = queue.dequeue();

        std::cout << "loading texture " << i << std::endl;
        std::cout << "x: " << texture.size.x() << std::endl;
        std::cout << "y: " << texture.size.y() << std::endl;
    }

    return 0;
}