#pragma once

#include <graphics/buffer/index_buffer.hpp>
#include <graphics/buffer/vertex_buffer.hpp>
#include <graphics/command/graphics_command.hpp>
#include <graphics/material/material.hpp>

namespace moka
{
    class draw_command final : public graphics_command
    {
    public:
        material_handle mat = std::numeric_limits<material_handle>::max();

        vertex_buffer vertex_buffer;
        uint32_t vertex_count = 0;
        uint32_t first_vertex = 0;

        index_buffer index_buffer;
        uint32_t index_count = 0;
        index_type idx_type = index_type::uint16;
        uint32_t index_buffer_offset = 0;

        primitive_type prim_type = primitive_type::triangles;

        draw_command();

        ~draw_command();

        draw_command(const draw_command& rhs);

        draw_command& operator=(const draw_command& rhs);

        draw_command(draw_command&& rhs) noexcept;

        draw_command& operator=(draw_command&& rhs) noexcept;

        void accept(graphics_visitor& visitor) override;

        draw_command& set_index_buffer_offset(uint32_t offset);

        draw_command& set_index_type(index_type type);

        draw_command& set_primitive_type(primitive_type prim_type);

        draw_command& set_index_count(uint32_t count);

        draw_command& set_first_vertex(uint32_t first);

        draw_command& set_vertex_count(uint32_t count);

        draw_command& set_material(moka::material_handle material);

        draw_command& set_vertex_buffer(moka::vertex_buffer vertex_buffer);

        draw_command& set_index_buffer(moka::index_buffer index_buffer);
    };
} // namespace moka
