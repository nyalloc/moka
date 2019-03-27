#pragma once

#include <cstdint>
#include <limits>

namespace moka
{
    /**
     * \brief The type of index buffer being used.
     */
    enum class index_type
    {
        int8,
        int16,
        int32,
        int64,
        uint8,
        uint16,
        uint32,
        uint64,
        float32
    };

    /**
     * \brief A handle to a index buffer object on the device.
     */
    struct index_buffer_handle final
    {
        uint16_t id = std::numeric_limits<uint16_t>::max();

        bool operator==(const index_buffer_handle& rhs) const;

        bool operator!=(const index_buffer_handle& rhs) const;

        bool operator>(const index_buffer_handle& rhs) const;

        bool operator<(const index_buffer_handle& rhs) const;

        bool operator>=(const index_buffer_handle& rhs) const;

        bool operator<=(const index_buffer_handle& rhs) const;
    };
} // namespace moka
