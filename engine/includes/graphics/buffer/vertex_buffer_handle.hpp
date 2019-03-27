#pragma once

#include <cstdint>
#include <limits>

namespace moka
{
    /**
     * \brief A handle to a vertex buffer object on the device.
     */
    struct vertex_buffer_handle final
    {
        uint16_t id = std::numeric_limits<uint16_t>::max();

        bool operator==(const vertex_buffer_handle& rhs) const;

        bool operator!=(const vertex_buffer_handle& rhs) const;

        bool operator>(const vertex_buffer_handle& rhs) const;

        bool operator<(const vertex_buffer_handle& rhs) const;

        bool operator>=(const vertex_buffer_handle& rhs) const;

        bool operator<=(const vertex_buffer_handle& rhs) const;
    };
} // namespace moka
