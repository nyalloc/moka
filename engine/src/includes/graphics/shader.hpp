#pragma once

#include <cstdint>

namespace moka
{
    /**
     * \brief The shader type.
     */
    enum class shader_type
    {
        vertex,
        fragment,
        compute
    };

    /**
     * \brief A handle to a shader object on the device.
     */
    struct shader_handle
    {
        uint16_t id;

        bool operator==(const shader_handle& rhs) const
        {
            return id == rhs.id;
        }

        bool operator!=(const shader_handle& rhs) const
        {
            return id != rhs.id;
        }

        bool operator>(const shader_handle& rhs) const
        {
            return id > rhs.id;
        }

        bool operator<(const shader_handle& rhs) const
        {
            return id < rhs.id;
        }

        bool operator>=(const shader_handle& rhs) const
        {
            return id >= rhs.id;
        }

        bool operator<=(const shader_handle& rhs) const
        {
            return id <= rhs.id;
        }
    };
} // namespace moka
