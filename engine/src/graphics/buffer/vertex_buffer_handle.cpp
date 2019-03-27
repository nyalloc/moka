#include <graphics/buffer/vertex_buffer_handle.hpp>

namespace moka
{
    bool vertex_buffer_handle::operator==(const vertex_buffer_handle& rhs) const
    {
        return id == rhs.id;
    }

    bool vertex_buffer_handle::operator!=(const vertex_buffer_handle& rhs) const
    {
        return id != rhs.id;
    }

    bool vertex_buffer_handle::operator>(const vertex_buffer_handle& rhs) const
    {
        return id > rhs.id;
    }

    bool vertex_buffer_handle::operator<(const vertex_buffer_handle& rhs) const
    {
        return id < rhs.id;
    }

    bool vertex_buffer_handle::operator>=(const vertex_buffer_handle& rhs) const
    {
        return id >= rhs.id;
    }

    bool vertex_buffer_handle::operator<=(const vertex_buffer_handle& rhs) const
    {
        return id <= rhs.id;
    }
} // namespace moka
