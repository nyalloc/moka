#include <graphics/command/sort_key.hpp>

namespace moka
{
    sort_key::sort_key() = default;

    sort_key::sort_key(const moka::program program, const alpha_mode alpha_mode, const float depth)
        : key_(
              static_cast<uint64_t>(depth * std::numeric_limits<uint16_t>::max()) |
              static_cast<uint64_t>(alpha_mode) << 16 |
              static_cast<uint64_t>(program.id) << 24 |
              static_cast<uint64_t>(program.id) << 40 |
              static_cast<uint64_t>(program.id) << 48 |
              static_cast<uint64_t>(program.id) << 56)
    {
    }

    program sort_key::program() const
    {
        return {static_cast<uint16_t>(key_)};
    }

    bool sort_key::operator>(const sort_key& rhs) const
    {
        return key_ > rhs.key_;
    }

    bool sort_key::operator<(const sort_key& rhs) const
    {
        return key_ < rhs.key_;
    }

    bool sort_key::operator<=(const sort_key& rhs) const
    {
        return key_ <= rhs.key_;
    }

    bool sort_key::operator>=(const sort_key& rhs) const
    {
        return key_ >= rhs.key_;
    }

    bool sort_key::operator==(const sort_key& rhs) const
    {
        return key_ == rhs.key_;
    }
} // namespace moka
