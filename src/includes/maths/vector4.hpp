#pragma once

#include <array>
#include <maths/utilities.hpp>

namespace loki
{
    namespace detail
    {
        /**
        * \brief Representation of 4D vectors and points.
        *        This structure is used throughout Loki to pass 4D positions and directions around.
        *        It also contains functions for doing common vector operations.
        * \tparam T Type used to represent the vector's XYZW properties.
        */
        template<typename T>
        class vector4
        {
            std::array<T, 4> components;
        public:
            /*----Constructors-----------------------------------*/

            constexpr vector4(const vector4<T>& rhs) noexcept;
            constexpr vector4(vector4<T>&& rhs) noexcept;

            constexpr explicit vector4(const T val);
            template<typename D> constexpr explicit vector4(const D val);

            constexpr vector4(const T x, const T y, const T z, const T w);
            template<typename D> constexpr vector4(const D x, const D y, const D z, const D w);

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
            * \brief Access the Z element of the vector.
            * \return Z
            */
            constexpr const T& z() const;

            /**
            * \brief Access the W element of the vector.
            * \return W
            */
            constexpr const T& w() const;

            /**
            * \brief Access the element indicated by the index. { 0, 1, 2, 3 } == { X, Y, Z, W }
            * \param index The index [0-3] that specifies which element you wish to access.
            * \return The element indicated by the index.
            */
            constexpr T& operator[](const size_t index);

            /**
            * \brief Access the element indicated by the index. { 0, 1, 2, 3 } == { X, Y, Z, W }
            * \param index The index [0-3] that specifies which element you wish to access.
            * \return The element indicated by the index.
            */
            constexpr const T& operator[](const size_t index) const;

            /*----Member operators-------------------------------*/

            /**
            * \brief Returns true if two vectors are approximately equal.
            * \param rhs The vector being compared against this.
            * \return True if the vectors are approximately equal, otherwise false.
            */
            constexpr bool operator ==(const vector4<T>& rhs) noexcept;

            constexpr const vector4<T>& operator =(vector4<T>&& rhs) noexcept;
            constexpr const vector4<T>& operator =(const vector4<T>& rhs) noexcept;

            constexpr const vector4<T>& operator *=(const T scalar) noexcept;
            constexpr const vector4<T>& operator /=(const T scalar) noexcept;

            constexpr const vector4<T>& operator *=(const vector4<T>& rhs) noexcept;
            constexpr const vector4<T>& operator /=(const vector4<T>& rhs) noexcept;

            constexpr const vector4<T>& operator +=(const vector4<T>& rhs) noexcept;
            constexpr const vector4<T>& operator -=(const vector4<T>& rhs) noexcept;

            /*----Utilities--------------------------------------*/

            constexpr T squared_magnitude() const noexcept;

            constexpr static T squared_magnitude(const vector4<T>& vector) noexcept;

            constexpr T magnitude() const noexcept;

            constexpr static T magnitude(const vector4<T>& vector) noexcept;

            constexpr vector4<T> normalize() const noexcept;

            static constexpr vector4<T> normalize(const vector4<T>& vector) noexcept;

            /*----Factories--------------------------------------*/

            /**
            * \brief Shorthand for writing vector4<T>{ 0, 0, 0, 0 }.
            * \return { 0, 0, 0, 0 }
            */
            constexpr static vector4<T> zero() noexcept;

            /**
            * \brief Shorthand for writing vector4<T>{ 1, 1, 1, 1 }.
            * \return { 1, 1, 1, 1 }
            */
            constexpr static vector4<T> one() noexcept;

            /**
            * \brief Shorthand for writing vector4<T>{ inf, inf, inf }.
            * \return { inf, inf, inf }
            */
            constexpr static vector4<T> infinity() noexcept;

            /**
            * \brief Shorthand for writing vector4<T>{ -inf, -inf, -inf }.
            * \return { -inf, -inf, -inf }
            */
            constexpr static vector4<T> negative_infinity() noexcept;
        };

        /*----Non-member operators---------------------------*/

        template<class T>
        std::ostream& operator<<(std::ostream& lhs, const vector4<T>& rhs) noexcept
        {
            return lhs << "{ " << rhs.x() << ", " << rhs.y() << ", " << rhs.z() << ", " << rhs.w() << " }";
        }

        template<class T>
        constexpr vector4<T> operator - (const vector4<T>& rhs) noexcept
        {
            return { -rhs.x(), -rhs.y(), -rhs.z(), -rhs.w() };
        }

        template<class T>
        constexpr vector4<T> operator + (const vector4<T>& lhs, const vector4<T>& rhs) noexcept
        {
            return { lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z(), lhs.w() + rhs.w() };
        }

        template<class T>
        constexpr vector4<T> operator - (const vector4<T>& lhs, const vector4<T>& rhs) noexcept
        {
            return lhs + (-rhs);
        }

        template<class T>
        constexpr vector4<T> operator * (const vector4<T>& lhs, const T rhs) noexcept
        {
            return { lhs.x() * rhs, lhs.y() * rhs, lhs.z() * rhs, lhs.w() * rhs };
        }

        template<class T>
        constexpr vector4<T> operator / (const vector4<T>& lhs, const T rhs) noexcept
        {
            return { lhs.x() / rhs, lhs.y() / rhs, lhs.z() / rhs, lhs.w() / rhs };
        }

        template<class T>
        constexpr bool operator == (const vector4<T>& lhs, const vector4<T>& rhs) noexcept
        {
            return vector4<T>::squared_magnitude(lhs - rhs) < vector4<T>::epsilon * vector4<T>::epsilon;
        }

        template<>
        constexpr bool operator == (const vector4<int>& lhs, const vector4<int>& rhs) noexcept
        {
            return lhs.x() == rhs.x() && lhs.y() == rhs.y() && lhs.z() == rhs.z() && lhs.w() == rhs.w();
        }

        template<class T>
        constexpr bool operator != (const vector4<T>& lhs, const vector4<T>& rhs) noexcept
        {
            return !(lhs == rhs);
        }

        /*----Non-member functions---------------------------*/

        template<class T>
        constexpr T dot(const vector4<T>& lhs, const vector4<T>& rhs) noexcept
        {
            return (lhs.x() * rhs.x()) + (lhs.y() * rhs.y()) + (lhs.z() * rhs.z()) + (lhs.w() * rhs.w());
        }

        template<class T>
        constexpr vector4<T> clamp_magnitude(const vector4<T>& vector, const T max)
        {
            if (vector.squared_magnitude() > max * max)
            {
                return vector.normalized() * max;
            }
            return vector;
        }

        template<class T>
        constexpr vector4<T> min(const vector4<T>& lhs, const vector4<T>& rhs) noexcept
        {
            return { std::min(lhs.x(), rhs.x()), std::min(lhs.y(), rhs.y()), std::min(lhs.z(), rhs.z()), std::min(lhs.w(), rhs.w()) };
        }

        template<class T>
        constexpr vector4<T> max(const vector4<T>& lhs, const vector4<T>& rhs) noexcept
        {
            return { std::max(lhs.x(), rhs.x()), std::max(lhs.y(), rhs.y()), std::max(lhs.z(), rhs.z()), std::max(lhs.w(), rhs.w()) };
        }

        template<class T>
        constexpr T angle(const vector4<T>& from, const vector4<T>& to)
        {
            return std::acos(loki::clamp(dot(from.normalize(), to.normalize()), -1.0f, 1.0f)) * maths::radians_to_degrees;
        }

        template<class T>
        constexpr vector4<T> project(const vector4<T>& vector, const vector4<T>& on_normal)
        {
            T sqr_mag = on_normal.squared_magnitude();
            if (sqr_mag < std::numeric_limits<T>::epsilon())
            {
                return {};
            }
            return on_normal * dot(vector, on_normal) / sqr_mag;
        }

        template<class T>
        constexpr vector4<T> project_on_plane(const vector4<T>& vector, const vector4<T>& plane_normal)
        {
            return vector - project(vector, plane_normal);
        }

        template<class T>
        constexpr vector4<T> lerp(const vector4<T>& lhs, const vector4<T>& rhs, const float amount) noexcept
        {
            return lhs + ((rhs - lhs) * amount);
        }

        template<class T>
        constexpr vector4<T> move_towards(const vector4<T>& current, const vector4<T>& target, const float distance_delta) noexcept
        {
            vector4<T> to_vector{ target - current };
            const float dist = to_vector.magnitude();
            if (dist <= distance_delta || dist < std::numeric_limits<T>::epsilon())
            {
                return target;
            }
            return current + to_vector / dist * distance_delta;
        }

        template<class T>
        constexpr vector4<T> reflect(const vector4<T>& in_direction, const vector4<T>& in_normal) noexcept
        {
            return -2.0f * dot(in_normal, in_direction) * in_normal + in_direction;
        }

        template<class T>
        constexpr T distance(const vector4<T>& a, const vector4<T>& b) noexcept
        {
            vector4<T> vec
            { 
                a.x() - b.x(), 
                a.y() - b.y(), 
                a.z() - b.z(), 
                a.w() - b.w() 
            };

            return static_cast<T>(
                std::sqrt(
                    (vec.x() * vec.x()) + 
                    (vec.y() * vec.y()) + 
                    (vec.z() * vec.z()) + 
                    (vec.w() * vec.w())
                )
            );
        }

        /*----Member function definitions--------------------*/

        template <typename T>
        constexpr vector4<T>::vector4(const vector4<T>& rhs) noexcept
            = default;

        template <typename T>
        constexpr vector4<T>::vector4(vector4<T>&& rhs) noexcept
            = default;

        template <typename T>
        constexpr vector4<T>::vector4(const T val)
            : components{ val, val, val }
        {}

        template <typename T>
        constexpr vector4<T>::vector4(const T x, const T y, const T z, const T w)
            : components{ x, y, z, w }
        {}

        template <typename T>
        template <typename D>
        constexpr vector4<T>::vector4(const D val)
            : vector4{ static_cast<T>(val) }
        {}

        template <typename T>
        template <typename D>
        constexpr vector4<T>::vector4(const D x, const D y, const D z, const D w)
            : vector4{ static_cast<T>(x), static_cast<T>(y), static_cast<T>(z), static_cast<T>(w) }
        {}

        template <typename T>
        constexpr const T& vector4<T>::x() const
        {
            return components[0];
        }

        template <typename T>
        constexpr const T& vector4<T>::y() const
        {
            return components[1];
        }

        template <typename T>
        constexpr const T& vector4<T>::z() const
        {
            return components[2];
        }

        template <typename T>
        constexpr const T& vector4<T>::w() const
        {
            return components[3];
        }

        template <typename T>
        constexpr T& vector4<T>::operator[](const size_t index)
        {
            return components[index];
        }

        template <typename T>
        constexpr const T& vector4<T>::operator[](const size_t index) const
        {
            return components[index];
        }

        template <typename T>
        constexpr bool vector4<T>::operator==(const vector4<T>& rhs) noexcept
        {
            return components == rhs.components;
        }

        template <typename T>
        constexpr const vector4<T>& vector4<T>::operator=(const vector4<T>& rhs) noexcept
        {
            components = rhs.components;
            return *this;
        }

        template <typename T>
        constexpr const vector4<T>& vector4<T>::operator=(vector4<T>&& rhs) noexcept
        {
            components = std::move(rhs.components);
            return *this;
        }

        template <typename T>
        constexpr const vector4<T>& vector4<T>::operator*=(const T s) noexcept
        {
            components[0] *= s;
            components[1] *= s;
            components[2] *= s;
            components[3] *= s;
            return *this;
        }

        template <typename T>
        constexpr const vector4<T>& vector4<T>::operator*=(const vector4<T>& rhs) noexcept
        {
            components[0] *= rhs.components[0];
            components[1] *= rhs.components[1];
            components[2] *= rhs.components[2];
            components[3] *= rhs.components[3];
            return *this;
        }

        template <typename T>
        constexpr const vector4<T>& vector4<T>::operator/=(const T s) noexcept
        {
            components[0] /= s;
            components[1] /= s;
            components[2] /= s;
            components[3] /= s;
            return *this;
        }

        template <typename T>
        constexpr const vector4<T>& vector4<T>::operator/=(const vector4<T>& rhs) noexcept
        {
            components[0] /= rhs.components[0];
            components[1] /= rhs.components[1];
            components[2] /= rhs.components[2];
            components[3] /= rhs.components[3];
            return *this;
        }

        template <typename T>
        constexpr T vector4<T>::squared_magnitude() const noexcept
        {
            return vector4<T>::squared_magnitude(*this);
        }

        template <typename T>
        constexpr T vector4<T>::squared_magnitude(const vector4<T>& vector) noexcept
        {
            return dot(vector, vector);
        }

        template <typename T>
        constexpr T vector4<T>::magnitude() const noexcept
        {
            return vector4<T>::magnitude(*this);
        }

        template <typename T>
        constexpr T vector4<T>::magnitude(const vector4<T>& vector) noexcept
        {
            return std::sqrt(squared_magnitude(vector));
        }

        template <typename T>
        constexpr vector4<T> vector4<T>::normalize() const noexcept
        {
            return *this / magnitude();
        }

        template <typename T>
        constexpr vector4<T> vector4<T>::normalize(const vector4<T>& vector) noexcept
        {
            return vector.normalize();
        }

        template <typename T>
        constexpr const vector4<T>& vector4<T>::operator+=(const vector4<T>& v) noexcept
        {
            components[0] += v.components[0];
            components[1] += v.components[1];
            components[2] += v.components[2];
            components[3] += v.components[3];
            return *this;
        }

        template <typename T>
        constexpr const vector4<T>& vector4<T>::operator-=(const vector4<T>& v) noexcept
        {
            return *this += -v;
        }

        template <typename T>
        constexpr vector4<T> vector4<T>::zero() noexcept
        {
            return vector4<T>{0};
        }

        template <typename T>
        constexpr vector4<T> vector4<T>::one() noexcept
        {
            return vector4<T>{1};
        }

        template <typename T>
        constexpr vector4<T> vector4<T>::infinity() noexcept
        {
            return vector4<T>{std::numeric_limits<T>::infinity()};
        }

        template <typename T>
        constexpr vector4<T> vector4<T>::negative_infinity() noexcept
        {
            return vector4<T>{-std::numeric_limits<T>::infinity()};
        }
    }

    using vector4 = detail::vector4<float>;
    using vector4_int = detail::vector4<int>;
    using vector4_uint = detail::vector4<unsigned>;
    using point4 = vector4_int;
}
