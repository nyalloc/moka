#pragma once

#include <cstdint>

namespace moka
{
    struct program
    {
        uint16_t id = std::numeric_limits<uint16_t>::max();

        bool operator==(const program& rhs)
        {
            return id == rhs.id;
        }

        bool operator!=(const program& rhs)
        {
            return id != rhs.id;
        }

        bool operator>(const program& rhs)
        {
            return id > rhs.id;
        }

        bool operator<(const program& rhs)
        {
            return id < rhs.id;
        }

        bool operator>=(const program& rhs)
        {
            return id >= rhs.id;
        }

        bool operator<=(const program& rhs)
        {
            return id <= rhs.id;
        }
    };
} // namespace moka
