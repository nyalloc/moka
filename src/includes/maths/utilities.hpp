#pragma once

#include <algorithm>
#include <valarray>

namespace loki
{
    template <typename T>
    constexpr T clamp(const T& n, const T& lower, const T& upper) noexcept
    {
        return std::max(lower, std::min(n, upper));
    }

    template <typename T>
    constexpr T lerp(const T& start, const T& end, const float percent) noexcept
    {
        return(start + clamp(percent, 0.0f, 1.0f) * (end - start));
    }
    
    template <typename T>
    constexpr T lerp_unclamped(const T& start, const T& end, const float percent) noexcept
    {
        return(start + percent * (end - start));
    }

    class maths
    {
    public:
        static constexpr double pi = 3.14159265358979323846264338327950288;
        static constexpr double degrees_to_radians = pi * 2.0f / 360.0f;
        static constexpr double radians_to_degrees = 1.0f / degrees_to_radians;

        template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
        constexpr static T to_degrees(const T radians) noexcept
        {
            return radians * static_cast<T>(radians_to_degrees);
        }

        template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
        constexpr static T to_radians(const T degrees) noexcept
        {
            return degrees * static_cast<T>(degrees_to_radians);
        }
    };
}
