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

    frame_buffer_builder::frame_buffer_builder(graphics_device& device) : device_(device)
    {
    }

    frame_buffer_builder& frame_buffer_builder::add_depth_attachment(const frame_format format, const int width, const int height)
    {
        render_attachments.emplace_back(render_texture_data{frame_attachment::depth, format, width, height});
        return *this;
    }

    frame_buffer_builder& frame_buffer_builder::add_color_attachment(const frame_format format, const int width, const int height)
    {
        render_attachments.emplace_back(render_texture_data{frame_attachment::color, format, width, height});
        return *this;
    }

    frame_buffer_builder& frame_buffer_builder::add_stencil_attachment(const frame_format format, const int width, const int height)
    {
        render_attachments.emplace_back(render_texture_data{frame_attachment::stencil, format, width, height});
        return *this;
    }

    frame_buffer_builder& frame_buffer_builder::add_depth_stencil_attachment(
        const frame_format format, const int width, const int height)
    {
        render_attachments.emplace_back(
            render_texture_data{frame_attachment::depth_stencil, format, width, height});
        return *this;
    }

    frame_buffer_handle frame_buffer_builder::build()
    {
        return device_.make_frame_buffer(render_attachments.data(), render_attachments.size());
    }
} // namespace moka
