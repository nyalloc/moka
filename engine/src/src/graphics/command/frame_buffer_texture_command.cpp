#include <graphics/command/frame_buffer_texture_command.hpp>
#include <graphics/device/graphics_visitor.hpp>

namespace moka
{
    frame_buffer_texture_command::~frame_buffer_texture_command() = default;

    void frame_buffer_texture_command::accept(graphics_visitor& visitor)
    {
        visitor.visit(*this);
    }

    frame_buffer_texture_command& frame_buffer_texture_command::set_texture(const texture texture)
    {
        tex = texture;
        return *this;
    }

    frame_buffer_texture_command& frame_buffer_texture_command::set_attachment(const frame_attachment attachment)
    {
        this->attachment = attachment;
        return *this;
    }

    frame_buffer_texture_command& frame_buffer_texture_command::set_target(const image_target target)
    {
        this->target = target;
        return *this;
    }

    frame_buffer_texture_command& frame_buffer_texture_command::set_mip_level(const int level)
    {
        this->level = level;
        return *this;
    }
} // namespace moka
