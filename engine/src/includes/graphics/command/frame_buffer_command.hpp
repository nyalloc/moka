#pragma once

#include <graphics/buffer/frame_buffer_handle.hpp>
#include <graphics/command/graphics_command.hpp>

namespace moka
{
    class frame_buffer_command final : public graphics_command
    {
    public:
        frame_buffer_handle buffer;

        frame_buffer_command();

        ~frame_buffer_command();

        frame_buffer_command(const frame_buffer_command& rhs);

        frame_buffer_command& operator=(const frame_buffer_command& rhs);

        frame_buffer_command(frame_buffer_command&& rhs) noexcept;

        frame_buffer_command& operator=(frame_buffer_command&& rhs) noexcept;

        void accept(graphics_visitor& visitor) override;

        frame_buffer_command& set_frame_buffer(frame_buffer_handle buffer);
    };
} // namespace moka
