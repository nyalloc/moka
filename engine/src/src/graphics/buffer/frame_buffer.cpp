#include <graphics/buffer/frame_buffer_handle.hpp>
#include <graphics/device/graphics_device.hpp>

namespace moka
{
    bool frame_buffer_handle::operator==(const frame_buffer_handle& rhs) const
    {
        return id == rhs.id;
    }

    bool frame_buffer_handle::operator!=(const frame_buffer_handle& rhs) const
    {
        return id != rhs.id;
    }

    bool frame_buffer_handle::operator>(const frame_buffer_handle& rhs) const
    {
        return id > rhs.id;
    }

    bool frame_buffer_handle::operator<(const frame_buffer_handle& rhs) const
    {
        return id < rhs.id;
    }

    bool frame_buffer_handle::operator>=(const frame_buffer_handle& rhs) const
    {
        return id >= rhs.id;
    }

    bool frame_buffer_handle::operator<=(const frame_buffer_handle& rhs) const
    {
        return id <= rhs.id;
    }

    frame_buffer_handle frame_buffer_builder::build()
    {
        return device_.make_frame_buffer(
            render_attachments.data(), render_attachments.size());
    }
} // namespace moka
