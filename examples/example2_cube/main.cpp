
#include <asset_importer/resource_manager.hpp>
#include <application/application.hpp>
#include <asset_importer/filesystem.hpp>
#include <asset_importer/texture_importer/texture_importer.hpp>

int main()
{
    const auto path = moka::application::data_path()
        / ".." / ".." / ".." / "examples" / "resources";

    moka::resource_manager manager{ path };

    {
        // texture is alive
        auto texture = manager.load_asset<moka::texture_2d>("tile.png");
        std::cout << manager.size<moka::texture_2d>() << std::endl;
        std::cout << "loading texture" << std::endl;
        std::cout << "texture size: " << texture.size() << std::endl;
    }

    // texture has gone out of scope, resource_manager should automatically deallocate any shared resources

    std::cout << manager.size<moka::texture_2d>() << std::endl;

    return 0;
}
