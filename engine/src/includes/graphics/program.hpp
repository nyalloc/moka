#pragma once

#include <cstdint>

namespace moka
{
    /**
     * \brief A handle to a program object on the device.
     */
    struct program_handle
    {
        uint16_t id = std::numeric_limits<uint16_t>::max();

        bool operator==(const program_handle& rhs) const
        {
            return id == rhs.id;
        }

        bool operator!=(const program_handle& rhs) const
        {
            return id != rhs.id;
        }

        bool operator>(const program_handle& rhs) const
        {
            return id > rhs.id;
        }

        bool operator<(const program_handle& rhs) const
        {
            return id < rhs.id;
        }

        bool operator>=(const program_handle& rhs) const
        {
            return id >= rhs.id;
        }

        bool operator<=(const program_handle& rhs) const
        {
            return id <= rhs.id;
        }
    };
} // namespace moka
