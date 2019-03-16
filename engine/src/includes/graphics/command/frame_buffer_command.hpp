#pragma once

#include <graphics/buffer/frame_buffer_handle.hpp>
#include <graphics/command/graphics_command.hpp>

namespace moka
{
    /**
     * \brief Set the current frame buffer. This will modify all subsequent draw commands in this list.
     */
    class frame_buffer_command final : public graphics_command
    {
    public:
        frame_buffer_handle buffer; /**< The current frame buffer you want to use. */

        frame_buffer_command();

        ~frame_buffer_command();

        frame_buffer_command(const frame_buffer_command& rhs);

        frame_buffer_command& operator=(const frame_buffer_command& rhs);

        frame_buffer_command(frame_buffer_command&& rhs) noexcept;

        frame_buffer_command& operator=(frame_buffer_command&& rhs) noexcept;

        /**
         * \brief Accept a graphics_visitor object. Invoke this command using the graphics_visitor.
         * \param visitor A graphics_visitor object.
         */
        void accept(graphics_visitor& visitor) override;

        /**
         * \brief Set the current frame buffer.
         * \param buffer Set the current frame buffer.
         * \return A reference to this frame_buffer_command object to enable method chaining.
         */
        frame_buffer_command& set_frame_buffer(frame_buffer_handle buffer);
    };
} // namespace moka
