#pragma once

#include <cstdint>
#include <graphics/api/graphics_api.hpp>

namespace moka
{
    class sort_key final
    {
        uint64_t key_ = 0;

    public:
        sort_key();

        explicit sort_key(program program, alpha_mode alpha_mode, float depth);

        program program() const;

        bool operator>(const sort_key& rhs) const;

        bool operator<(const sort_key& rhs) const;

        bool operator<=(const sort_key& rhs) const;

        bool operator>=(const sort_key& rhs) const;

        bool operator==(const sort_key& rhs) const;
    };

} // namespace moka
