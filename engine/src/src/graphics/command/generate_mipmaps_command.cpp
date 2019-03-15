#include <graphics/command/generate_mipmaps_command.hpp>
#include <graphics/device/graphics_visitor.hpp>

namespace moka
{
    generate_mipmaps_command::~generate_mipmaps_command() = default;

    void generate_mipmaps_command::accept(graphics_visitor& visitor)
    {
        visitor.visit(*this);
    }

    generate_mipmaps_command& generate_mipmaps_command::set_texture(const texture texture)
    {
        tex = texture;
        return *this;
    }
} // namespace moka
