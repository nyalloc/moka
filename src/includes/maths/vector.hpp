#pragma once

#include <array>

namespace loki
{
    template<typename T>
    static constexpr T clamp(const T n, const T lower, const T upper) noexcept
    {
        return std::max(lower, std::min(n, upper));
    }

    template<typename T>
    static constexpr T lerp(const T start, const T end, const float percent) noexcept
    {
        return(start + percent * (end - start));
    }

    static constexpr uint8_t to_byte(const float val) noexcept
    {
        return static_cast<uint8_t>(val * 0xFF);
    }

    static constexpr float to_float(const uint8_t val) noexcept
    {
        return static_cast<float>(val) / 0xFF;
    }

    template<typename T, size_t size, typename = std::enable_if<std::is_arithmetic<T>::value>>
    class vector
    {
        constexpr static T epsilon = std::numeric_limits<T>::epsilon();

        std::array<T, size> components;

    public:

        template<typename = std::enable_if<size >= 2>>
        constexpr vector(const T x, const T y) noexcept
            : components{ x, y }
        {}

        template<typename = std::enable_if<size >= 3>>
        constexpr vector(const T x, const T y, const T z) noexcept
            : components{ x, y, z }
        {}

        template<typename = std::enable_if<size == 4>>
        constexpr vector(const T x, const T y, const T z, const T w) noexcept
            : components{ x, y, z, w }
        {}

        constexpr T x() const noexcept
        {
            return components[0];
        }

        template<typename = std::enable_if<size >= 2>>
        constexpr T y() const noexcept
        {
            return components[1];
        }

        template<typename = std::enable_if<size >= 3>>
        constexpr T z() const noexcept
        {
            return components[2];
        }

        template<typename = std::enable_if<size >= 4>>
        constexpr T w() const noexcept
        {
            return components[3];
        }

        template<typename = std::enable_if<std::is_same_v<T, uint8_t>>>
        constexpr float r() const noexcept
        {
            return to_float(components[0]);
        }

        template<typename = std::enable_if<size >= 2 && std::is_same_v<T, uint8_t>>>
        constexpr float g() const noexcept
        {
            return to_float(components[1]);
        }

        template<typename = std::enable_if<size >= 3 && std::is_same_v<T, uint8_t>>>
        constexpr float b() const noexcept
        {
            return to_float(components[2]);
        }

        template<typename = std::enable_if<size >= 4 && std::is_same_v<T, uint8_t>>>
        constexpr float a() const noexcept
        {
            return to_float(components[3]);
        }

        constexpr T& operator[](const size_t index)
        {
            return components[index];
        }

        constexpr const T& operator[](const size_t index) const
        {
            return components[index];
        }
    };

    template<typename T, size_t size>
    std::ostream& operator<<(std::ostream& lhs, const vector<T, size>& rhs) noexcept
    {
        lhs << "{ ";
        for (auto i = 0; i < size; i++)
        {
            lhs << rhs[i] << ", ";
        }
        lhs << " }";
        return lhs;
    }

    template<typename T, size_t size, typename = typename std::enable_if<size == 3>::type>
    constexpr static vector<T, size> cross(const vector<T, size>& lhs, const vector<T, size>& rhs) noexcept
    {
        return
        {
            lhs.y() * rhs.z() - lhs.z() * rhs.y(),
            lhs.z() * rhs.x() - lhs.x() * rhs.z(),
            lhs.x() * rhs.y() - lhs.y() * rhs.x()
        };
    }

    template<typename T, size_t size>
    constexpr static T dot(const vector<T, size>& lhs, const vector<T, size>& rhs) noexcept
    {
        T result{};

        for (auto i = 0; i < size; i++)
        {
            result += lhs[i] * rhs[i];
        }

        return result;
    }

    using vector2 = vector<float, 2>;
    using vector3 = vector<float, 3>;
    using vector4 = vector<float, 4>;
}
