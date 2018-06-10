#pragma once

#include <array>

namespace loki
{
    namespace detail
    {
        template<typename T>
        class vector2
        {
            std::array<T, 2> component;
        public:
            constexpr vector2(const T x, const T y)
                : component{ x, y }
            {}

            constexpr T& x()
            {
                return component[0];
            }

            constexpr T& y()
            {
                return component[1];
            }

            constexpr const T& x() const
            {
                return component[0];
            }

            constexpr const T& y() const
            {
                return component[1];
            }

            constexpr T& operator[](const size_t index)
            {
                return component[index];
            }

            constexpr const T& operator[](const size_t index) const
            {
                return component[index];
            }
        };
    }

    using vector2 = detail::vector2<float>;
    using vector2_int = detail::vector2<int>;
    using vector2_uint = detail::vector2<unsigned>;
    using point2 = vector2_int;
}
