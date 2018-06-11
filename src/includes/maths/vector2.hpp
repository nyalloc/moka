#pragma once

#include <array>
#include <maths/utilities.hpp>

namespace loki
{
    namespace detail
    {
        /**
        * \brief Representation of 2D vectors and points.
        *        This structure is used throughout Loki to pass 2D positions and directions around.
        *        It also contains functions for doing common vector operations.
        * \tparam T Type used to represent the vector's XY properties.
        */
        template<typename T>
        class vector2
        {
            std::array<T, 2> components;
        public:
            /*----Constructors-----------------------------------*/

            constexpr vector2(const vector2<T>& rhs) noexcept;
            constexpr vector2(vector2<T>&& rhs) noexcept;

            constexpr explicit vector2(const T val);
            template<typename D> constexpr explicit vector2(const D val);

            constexpr vector2(const T x, const T y);
            template<typename D> constexpr vector2(const D x, const D y);

            /*----Accessors--------------------------------------*/

            /**
            * \brief Access the X element of the vector.
            * \return X
            */
            constexpr const T& x() const;

            /**
            * \brief Access the Y element of the vector.
            * \return Y
            */
            constexpr const T& y() const;

            /**
            * \brief Access the element indicated by the index. { 0, 1 } == { X, Y }
            * \param index The index [0-1] that specifies which element you wish to access.
            * \return The element indicated by the index.
            */
            constexpr const T& operator[](const size_t index) const;

            /**
            * \brief Access the element indicated by the index. { 0, 1 } == { X, Y }
            * \param index The index [0-1] that specifies which element you wish to access.
            * \return The element indicated by the index.
            */
            constexpr T& operator[](const size_t index);

            /*----Member operators-------------------------------*/

            /**
            * \brief Returns true if two vectors are approximately equal.
            * \param rhs The vector being compared against this.
            * \return True if the vectors are approximately equal, otherwise false.
            */
            constexpr bool operator ==(const vector2<T>& rhs) noexcept;

            constexpr const vector2<T>& operator =(vector2<T>&& rhs) noexcept;
            constexpr const vector2<T>& operator =(const vector2<T>& rhs) noexcept;

            constexpr const vector2<T>& operator *=(const T scalar) noexcept;
            constexpr const vector2<T>& operator /=(const T scalar) noexcept;

            constexpr const vector2<T>& operator *=(const vector2<T>& rhs) noexcept;
            constexpr const vector2<T>& operator /=(const vector2<T>& rhs) noexcept;

            constexpr const vector2<T>& operator +=(const vector2<T>& rhs) noexcept;
            constexpr const vector2<T>& operator -=(const vector2<T>& rhs) noexcept;

            /*----Utilities--------------------------------------*/

            constexpr T squared_magnitude() const noexcept;

            constexpr static T squared_magnitude(const vector2<T>& vector) noexcept;

            constexpr T magnitude() const noexcept;

            constexpr static T magnitude(const vector2<T>& vector) noexcept;

            constexpr vector2<T> normalize() const noexcept;

            static constexpr vector2<T> normalize(const vector2<T>& vector) noexcept;

            /*----Factories--------------------------------------*/

            /**
            * \brief Shorthand for writing vector2<T>{ 0, 0 }.
            * \return { 0, 0 }
            */
            constexpr static vector2<T> zero() noexcept;

            /**
            * \brief Shorthand for writing vector2<T>{ 1, 1 }.
            * \return { 1, 1 }
            */
            constexpr static vector2<T> one() noexcept;

            /**
            * \brief Shorthand for writing vector2<T>{ 0, 1 }.
            * \return { 0, 1 }
            */
            constexpr static vector2<T> up() noexcept;

            /**
            * \brief Shorthand for writing vector2<T>{ 0, -1 }.
            * \return { 0, -1 }
            */
            constexpr static vector2<T> down() noexcept;

            /**
            * \brief Shorthand for writing vector2<T>{ -1, 0 }.
            * \return { -1, 0 }
            */
            constexpr static vector2<T> left() noexcept;

            /**
            * \brief Shorthand for writing vector2<T>{ 1, 0 }.
            * \return { 1, 0 }
            */
            constexpr static vector2<T> right() noexcept;

            /**
            * \brief Shorthand for writing vector2<T>{ inf, inf }.
            * \return { inf, inf }
            */
            constexpr static vector2<T> infinity() noexcept;

            /**
            * \brief Shorthand for writing vector2<T>{ -inf, -inf }.
            * \return { -inf, -inf }
            */
            constexpr static vector2<T> negative_infinity() noexcept;
        };

        /*----Non-member operators---------------------------*/

        template<class T>
        std::ostream& operator<<(std::ostream& lhs, const vector2<T>& rhs) noexcept
        {
            return lhs << "{ " << rhs.x() << ", " << rhs.y() << " }";
        }

        template<class T>
        constexpr vector2<T> operator - (const vector2<T>& rhs) noexcept
        {
            return { -rhs.x(), -rhs.y() };
        }

        template<class T>
        constexpr vector2<T> operator + (const vector2<T>& lhs, const vector2<T>& rhs) noexcept
        {
            return { lhs.x() + rhs.x(), lhs.y() + rhs.y() };
        }

        template<class T>
        constexpr vector2<T> operator - (const vector2<T>& lhs, const vector2<T>& rhs) noexcept
        {
            return lhs + (-rhs);
        }

        template<class T>
        constexpr vector2<T> operator * (const vector2<T>& lhs, const T rhs) noexcept
        {
            return { lhs.x() * rhs, lhs.y() * rhs };
        }

        template<class T>
        constexpr vector2<T> operator / (const vector2<T>& lhs, const T rhs) noexcept
        {
            return { lhs.x() / rhs, lhs.y() / rhs };
        }

        template<class T>
        constexpr bool operator == (const vector2<T>& lhs, const vector2<T>& rhs) noexcept
        {
            return vector2<T>::squared_magnitude(lhs - rhs) < vector2<T>::epsilon * vector2<T>::epsilon;
        }

        template<>
        constexpr bool operator == (const vector2<int>& lhs, const vector2<int>& rhs) noexcept
        {
            return lhs.x() == rhs.x() && lhs.y() == rhs.y();
        }

        template<class T>
        constexpr bool operator != (const vector2<T>& lhs, const vector2<T>& rhs) noexcept
        {
            return !(lhs == rhs);
        }

        /*----Non-member functions---------------------------*/

        template<class T>
        constexpr T dot(const vector2<T>& lhs, const vector2<T>& rhs) noexcept
        {
            return (lhs.x() * rhs.x()) + (lhs.y() * rhs.y());
        }

        template<class T>
        constexpr vector2<T> clamp_magnitude(const vector2<T>& vector, const T max)
        {
            if (vector.squared_magnitude() > max * max)
            {
                return vector.normalized() * max;
            }
            return vector;
        }

        template<class T>
        constexpr vector2<T> min(const vector2<T>& lhs, const vector2<T>& rhs) noexcept
        {
            return { std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y) };
        }

        template<class T>
        constexpr vector2<T> max(const vector2<T>& lhs, const vector2<T>& rhs) noexcept
        {
            return { std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y) };
        }

        template<class T>
        constexpr T angle(const vector2<T>& from, const vector2<T>& to)
        {
            return std::acos(loki::clamp(dot(from.normalize(), to.normalize()), -1.0f, 1.0f)) * maths::radians_to_degrees;
        }

        template<class T>
        constexpr vector2<T> project(const vector2<T>& vector, const vector2<T>& on_normal)
        {
            T sqr_mag = on_normal.squared_magnitude();
            if (sqr_mag < std::numeric_limits<T>::epsilon())
            {
                return {};
            }
            return on_normal * dot(vector, on_normal) / sqr_mag;
        }

        template<class T>
        constexpr vector2<T> project_on_plane(const vector2<T>& vector, const vector2<T>& plane_normal)
        {
            return vector - project(vector, plane_normal);
        }

        template<class T>
        constexpr vector2<T> lerp(const vector2<T>& lhs, const vector2<T>& rhs, const float amount) noexcept
        {
            return lhs + ((rhs - lhs) * amount);
        }

        template<class T>
        constexpr vector2<T> move_towards(const vector2<T>& current, const vector2<T>& target, const float distance_delta) noexcept
        {
            vector2<T> to_vector{ target - current };
            const float dist = to_vector.magnitude();
            if (dist <= distance_delta || dist < std::numeric_limits<T>::epsilon())
            {
                return target;
            }
            return current + to_vector / dist * distance_delta;
        }

        template<class T>
        constexpr vector2<T> reflect(const vector2<T>& in_direction, const vector2<T>& in_normal) noexcept
        {
            return -2.0f * dot(in_normal, in_direction) * in_normal + in_direction;
        }

        template<class T>
        constexpr T distance(const vector2<T>& a, const vector2<T>& b) noexcept
        {
            vector2<T> vec{ a.x() - b.x(), a.y() - b.y() };
            return static_cast<T>(std::sqrt(vec.x() * vec.x() + vec.y() * vec.y()));
        }

        /*----Member function definitions--------------------*/

        template <typename T>
        constexpr vector2<T>::vector2(const vector2<T>& rhs) noexcept
            = default;

        template <typename T>
        constexpr vector2<T>::vector2(vector2<T>&& rhs) noexcept
            = default;

        template <typename T>
        constexpr vector2<T>::vector2(const T val)
            : components{ val, val }
        {}

        template <typename T>
        constexpr vector2<T>::vector2(const T x, const T y)
            : components{ x, y }
        {}

        template <typename T>
        template <typename D>
        constexpr vector2<T>::vector2(const D val)
            : vector2{ static_cast<T>(val) }
        {}

        template <typename T>
        template <typename D>
        constexpr vector2<T>::vector2(const D x, const D y)
            : vector2{ static_cast<T>(x), static_cast<T>(y) }
        {}

        template <typename T>
        constexpr const T& vector2<T>::x() const
        {
            return components[0];
        }

        template <typename T>
        constexpr const T& vector2<T>::y() const
        {
            return components[1];
        }

        template <typename T>
        constexpr const T& vector2<T>::operator[](const size_t index) const
        {
            return components[index];
        }

        template <typename T>
        constexpr T& vector2<T>::operator[](const size_t index)
        {
            return components[index];
        }

        template <typename T>
        constexpr bool vector2<T>::operator==(const vector2<T>& rhs) noexcept
        {
            return components == rhs.components;
        }

        template <typename T>
        constexpr const vector2<T>& vector2<T>::operator=(const vector2<T>& rhs) noexcept
        {
            components = rhs.components;
            return *this;
        }

        template <typename T>
        constexpr const vector2<T>& vector2<T>::operator=(vector2<T>&& rhs) noexcept
        {
            components = std::move(rhs.components);
            return *this;
        }

        template <typename T>
        constexpr const vector2<T>& vector2<T>::operator*=(const T s) noexcept
        {
            components[0] *= s;
            components[1] *= s;
            return *this;
        }

        template <typename T>
        constexpr const vector2<T>& vector2<T>::operator*=(const vector2<T>& rhs) noexcept
        {
            components[0] *= rhs.components[0];
            components[1] *= rhs.components[1];
            return *this;
        }

        template <typename T>
        constexpr const vector2<T>& vector2<T>::operator/=(const T s) noexcept
        {
            components[0] /= s;
            components[1] /= s;
            return *this;
        }

        template <typename T>
        constexpr const vector2<T>& vector2<T>::operator/=(const vector2<T>& rhs) noexcept
        {
            components[0] /= rhs.components[0];
            components[1] /= rhs.components[1];
            return *this;
        }

        template <typename T>
        constexpr T vector2<T>::squared_magnitude() const noexcept
        {
            return vector2<T>::squared_magnitude(*this);
        }

        template <typename T>
        constexpr T vector2<T>::squared_magnitude(const vector2<T>& vector) noexcept
        {
            return dot(vector, vector);
        }

        template <typename T>
        constexpr T vector2<T>::magnitude() const noexcept
        {
            return vector2<T>::magnitude(*this);
        }

        template <typename T>
        constexpr T vector2<T>::magnitude(const vector2<T>& vector) noexcept
        {
            return std::sqrt(squared_magnitude(vector));
        }

        template <typename T>
        constexpr vector2<T> vector2<T>::normalize() const noexcept
        {
            return *this / magnitude();
        }

        template <typename T>
        constexpr vector2<T> vector2<T>::normalize(const vector2<T>& vector) noexcept
        {
            return vector.normalize();
        }

        template <typename T>
        constexpr const vector2<T>& vector2<T>::operator+=(const vector2<T>& v) noexcept
        {
            components[0] += v.components[0];
            components[1] += v.components[1];
            return *this;
        }

        template <typename T>
        constexpr const vector2<T>& vector2<T>::operator-=(const vector2<T>& v) noexcept
        {
            return *this += -v;
        }

        template <typename T>
        constexpr vector2<T> vector2<T>::zero() noexcept
        {
            return vector2<T>{0};
        }

        template <typename T>
        constexpr vector2<T> vector2<T>::one() noexcept
        {
            return vector2<T>{1};
        }

        template <typename T>
        constexpr vector2<T> vector2<T>::up() noexcept
        {
            return { 0, 1 };
        }

        template <typename T>
        constexpr vector2<T> vector2<T>::down() noexcept
        {
            return { 0, -1 };
        }

        template <typename T>
        constexpr vector2<T> vector2<T>::left() noexcept
        {
            return { -1, 0 };
        }

        template <typename T>
        constexpr vector2<T> vector2<T>::right() noexcept
        {
            return { 1, 0 };
        }

        template <typename T>
        constexpr vector2<T> vector2<T>::infinity() noexcept
        {
            return vector2<T>{std::numeric_limits<T>::infinity()};
        }

        template <typename T>
        constexpr vector2<T> vector2<T>::negative_infinity() noexcept
        {
            return vector2<T>{-std::numeric_limits<T>::infinity()};
        }
    }

    using vector2 = detail::vector2<float>;
    using vector2_int = detail::vector2<int>;
    using vector2_uint = detail::vector2<unsigned>;
    using point2 = vector2_int;
}
