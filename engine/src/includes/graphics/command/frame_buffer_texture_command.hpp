#pragma once

#include <graphics/buffer/frame_buffer.hpp>
#include <graphics/command/graphics_command.hpp>
#include <graphics/texture.hpp>

namespace moka
{
    class frame_buffer_texture_command final : public graphics_command
    {
    public:
        texture tex;
        frame_attachment attachment;
        image_target target;
        int level;

        virtual ~frame_buffer_texture_command();

        void accept(graphics_visitor& visitor) override;

        frame_buffer_texture_command& set_texture(texture texture);

        frame_buffer_texture_command& set_attachment(frame_attachment attachment);

        frame_buffer_texture_command& set_target(image_target target);

        frame_buffer_texture_command& set_mip_level(int level);
    };
} // namespace moka
