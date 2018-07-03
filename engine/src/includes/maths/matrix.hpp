#pragma once

#include <array>
#include <maths/vector2.hpp>

namespace moka
{
    namespace detail
    {
        template<typename T>
        class matrix2
        {
            std::array<vector2<T>, 2> components_;
        public:
            constexpr matrix2() noexcept;

            constexpr static size_t size() noexcept;

            constexpr matrix2(matrix2<T>&& rhs) noexcept;

            constexpr matrix2(const matrix2<T>& rhs);

            constexpr vector2<T>& operator=(const matrix2<T>& rhs);

            constexpr vector2<T>& operator=(matrix2<T>&& rhs) noexcept;

            constexpr matrix2(const T m00, const T m01, const T m10, const T m11);

            constexpr matrix2(const vector2<T>& first, const vector2<T>& second);

            template<typename D>
            constexpr matrix2(const D m00, const D m01, const D m10, const D m11);

            template<typename D>
            constexpr matrix2(const vector2<D>& first, const vector2<D>& second);

            constexpr const T& m00() noexcept;

            constexpr const T& m01() noexcept;

            constexpr const T& m10() noexcept;

            constexpr const T& m11() noexcept;

            constexpr vector2<T> get_row(const size_t index) noexcept;

            constexpr vector2<T> get_column(const size_t index) noexcept;

            constexpr void set_row(const size_t index, const vector2<T>& row) noexcept;

            constexpr void set_column(const size_t index, const vector2<T>& column) noexcept;

            constexpr static matrix2<T> identity() noexcept;

            constexpr static matrix2<T> zero() noexcept;

            constexpr static matrix2<T> one() noexcept;

            constexpr vector2<T>& operator[](const size_t index);

            constexpr const vector2<T>& operator[](const size_t index) const;
        };

        template <typename T>
        constexpr matrix2<T>::matrix2() noexcept
            : matrix2{ 1, 0, 0, 1 }
        {}

        template <typename T>
        constexpr size_t matrix2<T>::size() noexcept
        {
            return 2 * 2;
        }

        template <typename T>
        constexpr matrix2<T>::matrix2(matrix2<T>&& rhs) noexcept
        : components_(std::move(rhs.components_))
        {}

        template <typename T>
        constexpr matrix2<T>::matrix2(const matrix2<T>& rhs)
        : components_(rhs.components_)
        {}

        template <typename T>
        constexpr vector2<T>& matrix2<T>::operator=(const matrix2<T>& rhs)
        {
            components_ = rhs.components_;
            return *this;
        }

        template <typename T>
        constexpr vector2<T>& matrix2<T>::operator=(matrix2<T>&& rhs) noexcept
        {
            components_ = std::move(rhs.components_);
            return *this;
        }

        template <typename T>
        constexpr matrix2<T>::matrix2(const T m00, const T m01, const T m10, const T m11)
            : components_{ { { m00, m01 }, { m10, m11 } } }
        {}

        template <typename T>
        constexpr matrix2<T>::matrix2(const vector2<T>& first, const vector2<T>& second)
            : matrix2(first[0], first[1], second[0], second[1])
        {}

        template <typename T>
        template <typename D>
        constexpr matrix2<T>::matrix2(const D m00, const D m01, const D m10, const D m11)
            : matrix2(static_cast<T>(m00), static_cast<T>(m01), static_cast<T>(m10), static_cast<T>(m11))
        {}

        template <typename T>
        template <typename D>
        constexpr matrix2<T>::matrix2(const vector2<D>& first, const vector2<D>& second)
            : matrix2(first[0], first[1], second[0], second[1])
        {}

        template <typename T>
        constexpr const T& matrix2<T>::m00() noexcept
        {
            return components_[0][0];
        }

        template <typename T>
        constexpr const T& matrix2<T>::m01() noexcept
        {
            return components_[0][1];
        }

        template <typename T>
        constexpr const T& matrix2<T>::m10() noexcept
        {
            return components_[1][0];
        }

        template <typename T>
        constexpr const T& matrix2<T>::m11() noexcept
        {
            return components_[1][1];
        }

        template <typename T>
        constexpr vector2<T> matrix2<T>::get_row(const size_t index) noexcept
        {
            return { components_[0][index], components_[1][index] };
        }

        template <typename T>
        constexpr vector2<T> matrix2<T>::get_column(const size_t index) noexcept
        {
            return { components_[index][0], components_[index][1] };
        }

        template <typename T>
        constexpr void matrix2<T>::set_row(const size_t index, const vector2<T>& row) noexcept
        {
            components_[0][index] = row[0];
            components_[1][index] = row[1];
        }

        template <typename T>
        constexpr void matrix2<T>::set_column(const size_t index, const vector2<T>& column) noexcept
        {
            components_[index][0] = column[0];
            components_[index][1] = column[1];
        }

        template <typename T>
        constexpr matrix2<T> matrix2<T>::identity() noexcept
        {
            return { 1, 0, 0, 1 };
        }

        template <typename T>
        constexpr matrix2<T> matrix2<T>::zero() noexcept
        {
            return { 0, 0, 0, 0 };
        }

        template <typename T>
        constexpr matrix2<T> matrix2<T>::one() noexcept
        {
            return { 1, 1, 1, 1 };
        }

        template <typename T>
        constexpr vector2<T>& matrix2<T>::operator[](const size_t index)
        {
            return components_[index];
        }

        template <typename T>
        constexpr const vector2<T>& matrix2<T>::operator[](const size_t index) const
        {
            return components_[index];
        }
    }

    using matrix2 = detail::matrix2<float>;
}
