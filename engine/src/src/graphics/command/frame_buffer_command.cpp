#include <graphics/command/frame_buffer_command.hpp>
#include <graphics/device/graphics_visitor.hpp>

namespace moka
{
    frame_buffer_command::frame_buffer_command() = default;

    frame_buffer_command::~frame_buffer_command() = default;

    frame_buffer_command::frame_buffer_command(const frame_buffer_command& rhs)
        : buffer(rhs.buffer)
    {
    }

    frame_buffer_command& frame_buffer_command::operator=(const frame_buffer_command& rhs)
    {
        buffer = rhs.buffer;
        return *this;
    }

    frame_buffer_command::frame_buffer_command(frame_buffer_command&& rhs) noexcept
        : buffer(rhs.buffer)
    {
    }

    frame_buffer_command& frame_buffer_command::operator=(frame_buffer_command&& rhs) noexcept
    {
        buffer = rhs.buffer;
        return *this;
    }

    void frame_buffer_command::accept(graphics_visitor& visitor)
    {
        visitor.visit(*this);
    }

    frame_buffer_command& frame_buffer_command::set_frame_buffer(const frame_buffer buffer)
    {
        this->buffer = buffer;
        return *this;
    }
} // namespace moka
