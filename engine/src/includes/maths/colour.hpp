#pragma once

#include <maths/vector4.hpp>
#include <maths/vector3.hpp>

namespace neon
{
    using byte = uint8_t;

    namespace detail
    {
        template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
        constexpr byte float_to_byte(const T val) noexcept
        {
            return static_cast<byte>(neon::clamp(val, static_cast<T>(0.0f), static_cast<T>(1.0f)) * std::numeric_limits<byte>::max());
        }

        template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
        constexpr byte int_to_byte(const T val) noexcept
        {
            return static_cast<byte>(neon::clamp(val, static_cast<T>(0), static_cast<T>(255)));
        }

        constexpr float byte_to_float(const byte val) noexcept
        {
            return static_cast<float>(val) / std::numeric_limits<byte>::max();
        }
    }

    class colour
    {
        detail::vector4<byte> rgba_bytes_;
    public:
        constexpr colour(const byte r, const byte g, const byte b, const byte a = static_cast<byte>(255)) noexcept
            : rgba_bytes_{ r, g, b, a }
        {}

        constexpr colour() noexcept
            : colour{ detail::int_to_byte(255), detail::int_to_byte(255), detail::int_to_byte(255), detail::int_to_byte(255) }
        {}

        constexpr colour(const vector3& colour, const float alpha = 1.0f) noexcept
            : colour{ detail::float_to_byte(colour.x()), detail::float_to_byte(colour.y()), detail::float_to_byte(colour.z()), detail::float_to_byte(alpha) }
        {}

        constexpr colour(const vector4& colour) noexcept
            : colour{ detail::float_to_byte(colour.x()), detail::float_to_byte(colour.y()), detail::float_to_byte(colour.z()), detail::float_to_byte(colour.w()) }
        {}

        constexpr colour(const int r, const int g, const int b, const int a = 255) noexcept
            : colour{ detail::int_to_byte(r), detail::int_to_byte(g), detail::int_to_byte(b), detail::int_to_byte(a) }
        {}

        constexpr colour(const float r, const float g, const float b, const float a = static_cast<float>(1.0f)) noexcept
            : colour{ detail::float_to_byte(r), detail::float_to_byte(g), detail::float_to_byte(b), detail::float_to_byte(a) }
        {}

        constexpr void r(const int val) noexcept
        {
            rgba_bytes_[0] = detail::int_to_byte(val);
        }

        constexpr void r(const float val) noexcept
        {
            rgba_bytes_[0] = detail::float_to_byte(val);
        }

        constexpr void g(const int val) noexcept
        {
            rgba_bytes_[1] = detail::int_to_byte(val);
        }

        constexpr void g(const float val) noexcept
        {
            rgba_bytes_[1] = detail::float_to_byte(val);
        }

        constexpr void b(const int val) noexcept
        {
            rgba_bytes_[2] = detail::int_to_byte(val);
        }

        constexpr void b(const float val) noexcept
        {
            rgba_bytes_[2] = detail::float_to_byte(val);
        }

        constexpr void a(const int val) noexcept
        {
            rgba_bytes_[3] = detail::int_to_byte(val);
        }

        constexpr void a(const float val) noexcept
        {
            rgba_bytes_[3] = detail::float_to_byte(val);
        }

        constexpr float r() const noexcept
        {
            return detail::byte_to_float(rgba_bytes_[0]);
        }

        constexpr float g() const noexcept
        {
            return detail::byte_to_float(rgba_bytes_[1]);
        }

        constexpr float b() const noexcept
        {
            return detail::byte_to_float(rgba_bytes_[2]);
        }

        constexpr float a() const noexcept
        {
            return detail::byte_to_float(rgba_bytes_[3]);
        }
    };

    using color = colour;
}
