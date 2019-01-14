#include <graphics/command/draw_command.hpp>

namespace moka
{
    draw_command::draw_command() = default;

    draw_command::~draw_command() = default;

    draw_command::draw_command(const draw_command& rhs)
        : mat(rhs.mat),
          vertex_buffer(rhs.vertex_buffer),
          vertex_count(rhs.vertex_count),
          first_vertex(rhs.first_vertex),
          index_buffer(rhs.index_buffer),
          index_count(rhs.index_count),
          idx_type(rhs.idx_type),
          index_buffer_offset(rhs.index_buffer_offset),
          prim_type(rhs.prim_type)
    {
    }

    draw_command& draw_command::operator=(const draw_command& rhs)
    {
        mat = rhs.mat;
        vertex_buffer = rhs.vertex_buffer;
        vertex_count = rhs.vertex_count;
        first_vertex = rhs.first_vertex;
        index_buffer = rhs.index_buffer;
        index_count = rhs.index_count;
        idx_type = rhs.idx_type;
        index_buffer_offset = rhs.index_buffer_offset;
        prim_type = rhs.prim_type;
        return *this;
    }

    draw_command::draw_command(draw_command&& rhs) noexcept
        : mat(std::move(rhs.mat)),
          vertex_buffer(rhs.vertex_buffer),
          vertex_count(rhs.vertex_count),
          first_vertex(rhs.first_vertex),
          index_buffer(rhs.index_buffer),
          index_count(rhs.index_count),
          idx_type(rhs.idx_type),
          index_buffer_offset(rhs.index_buffer_offset),
          prim_type(rhs.prim_type)
    {
    }

    draw_command& draw_command::operator=(draw_command&& rhs) noexcept
    {
        mat = std::move(rhs.mat);
        vertex_buffer = rhs.vertex_buffer;
        vertex_count = rhs.vertex_count;
        first_vertex = rhs.first_vertex;
        index_buffer = rhs.index_buffer;
        index_count = rhs.index_count;
        idx_type = rhs.idx_type;
        index_buffer_offset = rhs.index_buffer_offset;
        prim_type = rhs.prim_type;
        return *this;
    }

    void draw_command::accept(graphics_visitor& visitor)
    {
        visitor.visit(*this);
    }

    draw_command& draw_command::set_index_buffer_offset(uint32_t offset)
    {
        this->index_buffer_offset = offset;
        return *this;
    }

    draw_command& draw_command::set_index_type(index_type type)
    {
        this->idx_type = type;
        return *this;
    }

    draw_command& draw_command::set_primitive_type(primitive_type prim_type)
    {
        this->prim_type = prim_type;
        return *this;
    }

    draw_command& draw_command::set_index_count(uint32_t count)
    {
        this->index_count = count;
        return *this;
    }

    draw_command& draw_command::set_first_vertex(const uint32_t first)
    {
        this->first_vertex = first;
        return *this;
    }

    draw_command& draw_command::set_vertex_count(const uint32_t count)
    {
        this->vertex_count = count;
        return *this;
    }

    draw_command& draw_command::set_material(const material& material)
    {
        this->mat = material;
        return *this;
    }

    draw_command& draw_command::set_vertex_buffer(const moka::vertex_buffer vertex_buffer)
    {
        this->vertex_buffer = vertex_buffer;
        return *this;
    }

    draw_command& draw_command::set_index_buffer(moka::index_buffer index_buffer)
    {
        this->index_buffer = index_buffer;
        return *this;
    }
} // namespace moka
