#pragma once

#include <cstdint>

namespace moka
{
    enum class shader_type
    {
        vertex,
        fragment,
        compute
    };

    struct shader_handle
    {
        uint16_t id;

        bool operator==(const shader_handle& rhs)
        {
            return id == rhs.id;
        }

        bool operator!=(const shader_handle& rhs)
        {
            return id != rhs.id;
        }

        bool operator>(const shader_handle& rhs)
        {
            return id > rhs.id;
        }

        bool operator<(const shader_handle& rhs)
        {
            return id < rhs.id;
        }

        bool operator>=(const shader_handle& rhs)
        {
            return id >= rhs.id;
        }

        bool operator<=(const shader_handle& rhs)
        {
            return id <= rhs.id;
        }
    };
} // namespace moka
