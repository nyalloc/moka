#pragma once

#include <graphics/buffer/index_buffer_handle.hpp>
#include <graphics/command/graphics_command.hpp>

namespace moka
{
    /**
     * \brief Fill an index buffer.
     */
    class fill_index_buffer_command final : public graphics_command
    {
    public:
        index_buffer_handle handle; /**< The index buffer you want to fill. */
        const void* data;           /**< The host buffer of index data. */
        size_t size;                /**< The size of the host buffer. */

        virtual ~fill_index_buffer_command();

        /**
         * \brief Accept a graphics_visitor object. Invoke this command using the graphics_visitor.
         * \param visitor A graphics_visitor object.
         */
        void accept(graphics_visitor& visitor) override;

        /**
         * \brief Set the index buffer that you want to fill.
         * \param handle The index buffer you want to fill.
         * \param data The host buffer data you want to fill the index buffer data.
         * \param size The size of the host buffer data.
         * \return A reference to this fill_index_buffer_command object to enable method chaining.
         */
        fill_index_buffer_command& set_buffer(index_buffer_handle handle, const void* data, size_t size);
    };
} // namespace moka
