#pragma once

#include <graphics/command/graphics_command.hpp>
#include <graphics/texture.hpp>

namespace moka
{
    class generate_mipmaps_command final : public graphics_command
    {
    public:
        texture tex;

        virtual ~generate_mipmaps_command();

        void accept(graphics_visitor& visitor) override;

        generate_mipmaps_command& set_texture(texture t);
    };
} // namespace moka
