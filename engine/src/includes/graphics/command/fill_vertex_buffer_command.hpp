#pragma once

#include <graphics/buffer/vertex_buffer_handle.hpp>
#include <graphics/command/graphics_command.hpp>

namespace moka
{
    /**
     * \brief Fill a vertex buffer.
     */
    class fill_vertex_buffer_command final : public graphics_command
    {
    public:
        vertex_buffer_handle handle; /**< The vertex buffer you want to fill. */
        const void* data;            /**< The host buffer of vertex data. */
        size_t size;                 /**< The size of the host buffer. */

        virtual ~fill_vertex_buffer_command();

        /**
         * \brief Accept a graphics_visitor object. Invoke this command using the graphics_visitor.
         * \param visitor A graphics_visitor object.
         */
        void accept(graphics_visitor& visitor) override;

        /**
         * \brief Set the vertex buffer that you want to fill.
         * \param handle The vertex buffer you want to fill.
         * \param data The host buffer data you want to fill the vertex buffer data.
         * \param size The size of the host buffer data.
         * \return A reference to this fill_vertex_buffer_command object to enable method chaining.
         */
        fill_vertex_buffer_command& set_buffer(vertex_buffer_handle handle, const void* data, size_t size);
    };
} // namespace moka
