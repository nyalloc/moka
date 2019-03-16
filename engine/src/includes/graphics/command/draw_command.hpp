#pragma once

#include <graphics/buffer/index_buffer_handle.hpp>
#include <graphics/buffer/vertex_buffer_handle.hpp>
#include <graphics/command/graphics_command.hpp>
#include <graphics/material/material.hpp>

namespace moka
{
    /**
     * \brief Render primitives using the specified material. Contains vertex buffer, index buffer (optional) and material data.
     */
    class draw_command final : public graphics_command
    {
    public:
        material_handle material =
            std::numeric_limits<material_handle>::max(); /**< The material that should be use for rendering. */

        vertex_buffer_handle vertex_buffer; /**< The vertex buffer that should be used. */

        uint32_t vertex_count = 0; /**< The number of vertices to be rendered. */

        uint32_t first_vertex = 0; /**< Specifies the starting index in the enabled arrays. */

        index_buffer_handle index_buffer; /**< The index buffer that should be used. */

        uint32_t index_count = 0; /**< Specifies a pointer to the location where the indices are stored. */

        index_type idx_type = index_type::uint16; /**< Specifies the type of the values in the index buffer. */

        uint32_t index_buffer_offset =
            0; /**< Specifies the offset to the location where the indices are stored. */

        primitive_type prim_type = primitive_type::triangles; /**< Specifies what kind of primitives to render. */

        draw_command();

        ~draw_command();

        draw_command(const draw_command& rhs);

        draw_command& operator=(const draw_command& rhs);

        draw_command(draw_command&& rhs) noexcept;

        draw_command& operator=(draw_command&& rhs) noexcept;

        /**
         * \brief Accept a graphics_visitor object. Invoke this command using the graphics_visitor.
         * \param visitor A graphics_visitor object.
         */
        void accept(graphics_visitor& visitor) override;

        /**
         * \brief Set the index buffer offset.
         * \param offset The index buffer offset.
         * \return A reference to this draw_command object to enable method chaining.
         */
        draw_command& set_index_buffer_offset(uint32_t offset);

        /**
         * \brief Set the index type to use.
         * \param type The index type you want to use.
         * \return A reference to this draw_command object to enable method chaining.
         */
        draw_command& set_index_type(index_type type);

        /**
         * \brief Set the primitive type you want to use.
         * \param prim_type Set the primitive type you want to use.
         * \return A reference to this draw_command object to enable method chaining.
         */
        draw_command& set_primitive_type(primitive_type prim_type);

        /**
         * \brief Set the index count you want to use.
         * \param count The index count you want to use.
         * \return A reference to this draw_command object to enable method chaining.
         */
        draw_command& set_index_count(uint32_t count);

        /**
         * \brief Set the first vertex to use.
         * \param first The first vertex to use.
         * \return A reference to this draw_command object to enable method chaining.
         */
        draw_command& set_first_vertex(uint32_t first);

        /**
         * \brief Set the number of vertices in the vertex buffer.
         * \param count The number of vertices in the vertex buffer.
         * \return A reference to this draw_command object to enable method chaining.
         */
        draw_command& set_vertex_count(uint32_t count);

        /**
         * \brief Set the material to use when rendering.
         * \param material The material to use when rendering.
         * \return A reference to this draw_command object to enable method chaining.
         */
        draw_command& set_material(material_handle material);

        /**
         * \brief Set the vertex buffer you want to use.
         * \param vertex_buffer The vertex buffer you want to use.
         * \return A reference to this draw_command object to enable method chaining.
         */
        draw_command& set_vertex_buffer(vertex_buffer_handle vertex_buffer);

        /**
         * \brief Set the index buffer you want to use.
         * \param index_buffer The index buffer you want to use.
         * \return A reference to this draw_command object to enable method chaining.
         */
        draw_command& set_index_buffer(index_buffer_handle index_buffer);
    };
} // namespace moka
