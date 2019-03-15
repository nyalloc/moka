#include <graphics/buffer/frame_buffer.hpp>
#include <graphics/device/graphics_device.hpp>

namespace moka
{
    bool frame_buffer::operator==(const frame_buffer& rhs) const
    {
        return id == rhs.id;
    }

    bool frame_buffer::operator!=(const frame_buffer& rhs) const
    {
        return id != rhs.id;
    }

    bool frame_buffer::operator>(const frame_buffer& rhs) const
    {
        return id > rhs.id;
    }

    bool frame_buffer::operator<(const frame_buffer& rhs) const
    {
        return id < rhs.id;
    }

    bool frame_buffer::operator>=(const frame_buffer& rhs) const
    {
        return id >= rhs.id;
    }

    bool frame_buffer::operator<=(const frame_buffer& rhs) const
    {
        return id <= rhs.id;
    }

    frame_buffer frame_buffer_builder::build()
    {
        return device_.make_frame_buffer(
            render_attachments.data(), render_attachments.size());
    }
} // namespace moka
