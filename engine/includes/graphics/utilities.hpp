/*
===========================================================================
Moka Source Code
Copyright 2019 Stuart Adams. All rights reserved.
https://github.com/stuartdadams/moka
stuartdadams | linkedin.com/in/stuartdadams

This file is part of the Moka Real-Time Physically-Based Rendering Project.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

===========================================================================
*/
#pragma once

#include <algorithm>
#include <valarray>

#undef max
#undef min

namespace moka
{
    template <typename T>
    constexpr T clamp(const T& n, const T& lower, const T& upper) noexcept
    {
        return std::max(lower, std::min(n, upper));
    }

    template <typename T>
    constexpr T lerp(const T& start, const T& end, const float percent) noexcept
    {
        return (start + clamp(percent, 0.0f, 1.0f) * (end - start));
    }

    template <typename T>
    constexpr T lerp_unclamped(const T& start, const T& end, const float percent) noexcept
    {
        return (start + percent * (end - start));
    }

    class maths
    {
    public:
        static constexpr double pi = 3.14159265358979323846264338327950288;
        static constexpr double degrees_to_radians = pi * 2.0f / 360.0f;
        static constexpr double radians_to_degrees = 1.0f / degrees_to_radians;

        template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
        constexpr static T to_degrees(const T radians) noexcept
        {
            return radians * static_cast<T>(radians_to_degrees);
        }

        template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
        constexpr static T to_radians(const T degrees) noexcept
        {
            return degrees * static_cast<T>(degrees_to_radians);
        }
    };
} // namespace moka
