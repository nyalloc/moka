#pragma once

#include <cstdint>
#include <limits>
#include <vector>

namespace moka
{
    class graphics_device;

    // will add to this as it becomes necessary!
    enum class frame_format
    {
        depth_component24,
    };

    enum class frame_attachment
    {
        color,
        depth,
        stencil,
        depth_stencil
    };

    struct frame_buffer_handle final
    {
        uint16_t id = std::numeric_limits<uint16_t>::max();

        bool operator==(const frame_buffer_handle& rhs) const;

        bool operator!=(const frame_buffer_handle& rhs) const;

        bool operator>(const frame_buffer_handle& rhs) const;

        bool operator<(const frame_buffer_handle& rhs) const;

        bool operator>=(const frame_buffer_handle& rhs) const;

        bool operator<=(const frame_buffer_handle& rhs) const;
    };

    struct render_texture_data final
    {
        frame_attachment attachment;
        frame_format format;
        int width;
        int height;
    };

    struct frame_buffer_builder final
    {
        explicit frame_buffer_builder(graphics_device& device) : device_(device)
        {
        }

        std::vector<render_texture_data> render_attachments;

        frame_buffer_builder& add_depth_attachment(const frame_format format, const int width, const int height)
        {
            render_attachments.emplace_back(render_texture_data{
                frame_attachment::depth, format, width, height});
            return *this;
        }

        frame_buffer_builder& add_color_attachment(const frame_format format, const int width, const int height)
        {
            render_attachments.emplace_back(render_texture_data{
                frame_attachment::color, format, width, height});
            return *this;
        }

        frame_buffer_builder& add_stencil_attachment(
            const frame_format format, const int width, const int height)
        {
            render_attachments.emplace_back(render_texture_data{
                frame_attachment::stencil, format, width, height});
            return *this;
        }

        frame_buffer_builder& add_depth_stencil_attachment(
            const frame_format format, const int width, const int height)
        {
            render_attachments.emplace_back(render_texture_data{
                frame_attachment::depth_stencil, format, width, height});
            return *this;
        }

        frame_buffer_handle build();

    private:
        graphics_device& device_;
    };
} // namespace moka
