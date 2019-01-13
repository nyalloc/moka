#include <graphics/command/fill_index_buffer_command.hpp>

namespace moka
{
    fill_index_buffer_command::~fill_index_buffer_command() = default;

    void fill_index_buffer_command::accept(graphics_visitor& visitor)
    {
        visitor.visit(*this);
    }

    fill_index_buffer_command& fill_index_buffer_command::set_buffer(
        const index_buffer handle, const void* data, const size_t size)
    {
        this->handle = handle;
        this->data = data;
        this->size = size;
        return *this;
    }
} // namespace moka
