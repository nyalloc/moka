#include <graphics/command/fill_vertex_buffer_command.hpp>
#include <graphics/device/graphics_visitor.hpp>

namespace moka
{
    fill_vertex_buffer_command::~fill_vertex_buffer_command()
    {
    }

    void fill_vertex_buffer_command::accept(graphics_visitor& visitor)
    {
        visitor.visit(*this);
    }

    fill_vertex_buffer_command& fill_vertex_buffer_command::set_buffer(
        const vertex_buffer handle, const void* data, const size_t size)
    {
        this->handle = handle;
        this->data = data;
        this->size = size;
        return *this;
    }
} // namespace moka
