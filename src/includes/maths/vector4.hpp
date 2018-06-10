#pragma once

#include <array>

namespace loki
{
    namespace detail
    {
        template<typename T>
        class vector4
        {
            std::array<T, 4> component;
        public:
            constexpr vector4(const T x, const T y, const T z, const T w)
                : component{ x, y, z, w }
            {}

            constexpr T& x()
            {
                return component[0];
            }

            constexpr T& y()
            {
                return component[1];
            }

            constexpr T& z()
            {
                return component[2];
            }

            constexpr T& w()
            {
                return component[3];
            }

            constexpr const T& x() const
            {
                return component[0];
            }

            constexpr const T& y() const
            {
                return component[1];
            }

            constexpr const T& z() const
            {
                return component[2];
            }

            constexpr const T& w() const
            {
                return component[3];
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

    using vector4 = detail::vector4<float>;
    using vector4_int = detail::vector4<int>;
    using vector4_uint = detail::vector4<unsigned>;
    using point4 = vector4_int;
}
