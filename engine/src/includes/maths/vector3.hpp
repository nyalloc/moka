#pragma once

#include <array>
#include <maths/utilities.hpp>

#undef max
#undef min

namespace neon
{
    namespace detail
    {
        /**
         * \brief Representation of 3D vectors and points. 
         *        This structure is used throughout neon to pass 3D positions and directions around. 
         *        It also contains functions for doing common vector operations.
         * \tparam T Type used to represent the vector's XYZ properties.
         */
        template<typename T>
        class vector3
        {
            std::array<T, 3> components_;
        public:
            /*----Constructors-----------------------------------*/

            constexpr vector3(const vector3<T>& rhs) noexcept;
            constexpr vector3(vector3<T>&& rhs) noexcept;

            constexpr explicit vector3(const T val);
            template<typename D> constexpr explicit vector3(const D val);

            constexpr vector3(const T x, const T y, const T z);
            template<typename D> constexpr vector3(const D x, const D y, const D z);

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
             * \brief Access the element indicated by the index. { 0, 1, 2 } == { X, Y, Z }
             * \param index The index [0-2] that specifies which element you wish to access.
             * \return The element indicated by the index.
             */
            constexpr const T& operator[](const size_t index) const;

            /**
             * \brief Access the element indicated by the index. { 0, 1, 2 } == { X, Y, Z }
             * \param index The index [0-2] that specifies which element you wish to access.
             * \return The element indicated by the index.
             */
            constexpr T& operator[](const size_t index);

            /*----Member operators-------------------------------*/

            /**
             * \brief Returns true if two vectors are approximately equal.
             * \param rhs The vector being compared against this.
             * \return True if the vectors are approximately equal, otherwise false.
             */
            constexpr bool operator ==(const vector3<T>& rhs) noexcept;

            constexpr const vector3<T>& operator =(vector3<T>&& rhs) noexcept;
            constexpr const vector3<T>& operator =(const vector3<T>& rhs) noexcept;

            constexpr const vector3<T>& operator *=(const T scalar) noexcept;
            constexpr const vector3<T>& operator /=(const T scalar) noexcept;

            constexpr const vector3<T>& operator *=(const vector3<T>& rhs) noexcept;
            constexpr const vector3<T>& operator /=(const vector3<T>& rhs) noexcept;

            constexpr const vector3<T>& operator +=(const vector3<T>& rhs) noexcept;
            constexpr const vector3<T>& operator -=(const vector3<T>& rhs) noexcept;

            /*----Utilities--------------------------------------*/

            constexpr T squared_magnitude() const noexcept;

            constexpr static T squared_magnitude(const vector3<T>& vector) noexcept;

            constexpr T magnitude() const noexcept;

            constexpr static T magnitude(const vector3<T>& vector) noexcept;

            constexpr vector3<T> normalize() const noexcept;

            static constexpr vector3<T> normalize(const vector3<T>& vector) noexcept;

            /*----Factories--------------------------------------*/

            /**
             * \brief Shorthand for writing vector3<T>{ 0, 0, 0 }.
             * \return { 0, 0, 0 }
             */
            constexpr static vector3<T> zero() noexcept;

            /**
             * \brief Shorthand for writing vector3<T>{ 1, 1, 1 }.
             * \return { 1, 1, 1 }
             */
            constexpr static vector3<T> one() noexcept;

            /**
             * \brief Shorthand for writing vector3<T>{ 0, 1, 0 }.
             * \return { 0, 1, 0 }
             */
            constexpr static vector3<T> up() noexcept;

            /**
             * \brief Shorthand for writing vector3<T>{ 0, -1, 0 }.
             * \return { 0, -1, 0 }
             */
            constexpr static vector3<T> down() noexcept;

            /**
             * \brief Shorthand for writing vector3<T>{ -1, 0, 0 }.
             * \return { -1, 0, 0 }
             */
            constexpr static vector3<T> left() noexcept;

            /**
             * \brief Shorthand for writing vector3<T>{ 1, 0, 0 }.
             * \return { 1, 0, 0 }
             */
            constexpr static vector3<T> right() noexcept;

            /**
             * \brief Shorthand for writing vector3<T>{ 0, 0, 1 }.
             * \return { 0, 0, 1 }
             */
            constexpr static vector3<T> forward() noexcept;

            /**
             * \brief Shorthand for writing vector3<T>{ 0, 0, -1 }.
             * \return { 0, 0, -1 }
             */            
            constexpr static vector3<T> backward() noexcept;
       
            /**
             * \brief Shorthand for writing vector3<T>{ inf, inf, inf }.
             * \return { inf, inf, inf }
             */
            constexpr static vector3<T> infinity() noexcept;

            /**
             * \brief Shorthand for writing vector3<T>{ -inf, -inf, -inf }.
             * \return { -inf, -inf, -inf }
             */
            constexpr static vector3<T> negative_infinity() noexcept;
        };

        /*----Non-member operators---------------------------*/

        template<class T>
        std::ostream& operator<<(std::ostream& lhs, const vector3<T>& rhs) noexcept
        {
            return lhs << "{ " << rhs.x() << ", " << rhs.y() << ", " << rhs.z() << " }";
        }

        template<class T>
        constexpr vector3<T> operator - (const vector3<T>& rhs) noexcept
        {
            return { -rhs.x(), -rhs.y(), -rhs.z() };
        }

        template<class T>
        constexpr vector3<T> operator + (const vector3<T>& lhs, const vector3<T>& rhs) noexcept
        {
            return { lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z() };
        }

        template<class T>
        constexpr vector3<T> operator - (const vector3<T>& lhs, const vector3<T>& rhs) noexcept
        {
            return lhs + (-rhs);
        }

        template<class T>
        constexpr vector3<T> operator * (const vector3<T>& lhs, const T rhs) noexcept
        {
            return { lhs.x() * rhs, lhs.y() * rhs, lhs.z() * rhs };
        }

        template<class T>
        constexpr vector3<T> operator / (const vector3<T>& lhs, const T rhs) noexcept
        {
            return { lhs.x() / rhs, lhs.y() / rhs, lhs.z() / rhs };
        }

        template<class T>
        constexpr bool operator == (const vector3<T>& lhs, const vector3<T>& rhs) noexcept
        {
            return vector3<T>::squared_magnitude(lhs - rhs) < vector3<T>::epsilon * vector3<T>::epsilon;
        }

        template<>
        constexpr bool operator == (const vector3<int>& lhs, const vector3<int>& rhs) noexcept
        {
            return lhs.x() == rhs.x() && lhs.y() == rhs.y() && lhs.z() == rhs.z();
        }

        template<class T>
        constexpr bool operator != (const vector3<T>& lhs, const vector3<T>& rhs) noexcept
        {
            return !(lhs == rhs);
        }

        /*----Non-member functions---------------------------*/

        template<class T>
        constexpr T dot(const vector3<T>& lhs, const vector3<T>& rhs) noexcept
        {
            return (lhs.x() * rhs.x()) + (lhs.y() * rhs.y()) + (lhs.z() * rhs.z());
        }

        template<class T>
        constexpr vector3<T> clamp_magnitude(const vector3<T>& vector, const T max)
        {
            if (vector.squared_magnitude() > max * max)
            {
                return vector.normalized() * max;
            }
            return vector;
        }

        template<class T>
        constexpr vector3<T> min(const vector3<T>& lhs, const vector3<T>& rhs) noexcept
        {
            return { std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z) };
        }

        template<class T>
        constexpr vector3<T> max(const vector3<T>& lhs, const vector3<T>& rhs) noexcept
        {
            return { std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z) };
        }

        template<class T>
        constexpr T angle(const vector3<T>& from, const vector3<T>& to)
        {
            return std::acos(neon::clamp(dot(from.normalize(), to.normalize()), -1.0f, 1.0f)) * maths::radians_to_degrees;
        }

        template<class T>
        constexpr vector3<T> project(const vector3<T>& vector, const vector3<T>& on_normal)
        {
            T sqr_mag = on_normal.squared_magnitude();
            if (sqr_mag < std::numeric_limits<T>::epsilon())
            {
                return {};
            }
            return on_normal * dot(vector, on_normal) / sqr_mag;
        }

        template<class T>
        constexpr vector3<T> project_on_plane(const vector3<T>& vector, const vector3<T>& plane_normal)
        {
            return vector - project(vector, plane_normal);
        }

        template<class T>
        constexpr vector3<T> lerp(const vector3<T>& lhs, const vector3<T>& rhs, const float amount) noexcept
        {
            return lhs + ((rhs - lhs) * amount);
        }

        template<class T>
        constexpr vector3<T> cross(const vector3<T>& lhs, const vector3<T>& rhs) noexcept
        {
            return
            {
                lhs.y() * rhs.z() - lhs.z() * rhs.y(),
                lhs.z() * rhs.x() - lhs.x() * rhs.z(),
                lhs.x() * rhs.y() - lhs.y() * rhs.x()
            };
        }

        template<class T>
        constexpr vector3<T> move_towards(const vector3<T>& current, const vector3<T>& target, const float distance_delta) noexcept
        {
            vector3<T> to_vector{ target - current };
            const float dist = to_vector.magnitude();
            if (dist <= distance_delta || dist < std::numeric_limits<T>::epsilon())
            {
                return target;
            }
            return current + to_vector / dist * distance_delta;
        }

        template<class T>
        constexpr vector3<T> reflect(const vector3<T>& in_direction, const vector3<T>& in_normal) noexcept
        {
            return -2.0f * dot(in_normal, in_direction) * in_normal + in_direction;
        }

        template<class T>
        constexpr T distance(const vector3<T>& a, const vector3<T>& b) noexcept
        {
            vector3<T> vec{ a.x() - b.x(), a.y() - b.y(), a.z() - b.z() };
            return static_cast<T>(std::sqrt(vec.x() * vec.x() + vec.y() * vec.y() + vec.z() * vec.z()));
        }

        /*----Member function definitions--------------------*/

        template <typename T>
        constexpr vector3<T>::vector3(const vector3<T>& rhs) noexcept
            = default;

        template <typename T>
        constexpr vector3<T>::vector3(vector3<T>&& rhs) noexcept
            = default;

        template <typename T>
        constexpr vector3<T>::vector3(const T val)
        : components_{val, val, val}
        {}

        template <typename T>
        constexpr vector3<T>::vector3(const T x, const T y, const T z)
        : components_{x, y, z}
        {}

        template <typename T>
        template <typename D>
        constexpr vector3<T>::vector3(const D val)
        : vector3{static_cast<T>(val)}
        {
        }

        template <typename T>
        template <typename D>
        constexpr vector3<T>::vector3(const D x, const D y, const D z)
        : vector3{static_cast<T>(x), static_cast<T>(y), static_cast<T>(z)}
        {
        }

        template <typename T>
        constexpr const T& vector3<T>::x() const
        {
            return components_[0];
        }

        template <typename T>
        constexpr const T& vector3<T>::y() const
        {
            return components_[1];
        }

        template <typename T>
        constexpr const T& vector3<T>::z() const
        {
            return components_[2];
        }

        template <typename T>
        constexpr const T& vector3<T>::operator[](const size_t index) const
        {
            return components_[index];
        }

        template <typename T>
        constexpr T& vector3<T>::operator[](const size_t index)
        {
            return components_[index];
        }

        template <typename T>
        constexpr bool vector3<T>::operator==(const vector3<T>& rhs) noexcept
        {
            return components_ == rhs.components_;
        }

        template <typename T>
        constexpr const vector3<T>& vector3<T>::operator=(const vector3<T>& rhs) noexcept
        {
            components_ = rhs.components_;
            return *this;
        }

        template <typename T>
        constexpr const vector3<T>& vector3<T>::operator=(vector3<T>&& rhs) noexcept
        {
            components_ = std::move(rhs.components_);
            return *this;
        }

        template <typename T>
        constexpr const vector3<T>& vector3<T>::operator*=(const T s) noexcept
        {
            components_[0] *= s;
            components_[1] *= s;
            components_[2] *= s;
            return *this;
        }

        template <typename T>
        constexpr const vector3<T>& vector3<T>::operator*=(const vector3<T>& rhs) noexcept
        {
            components_[0] *= rhs.components_[0];
            components_[1] *= rhs.components_[1];
            components_[2] *= rhs.components_[2];
            return *this;
        }

        template <typename T>
        constexpr const vector3<T>& vector3<T>::operator/=(const T s) noexcept
        {
            components_[0] /= s;
            components_[1] /= s;
            components_[2] /= s;
            return *this;
        }

        template <typename T>
        constexpr const vector3<T>& vector3<T>::operator/=(const vector3<T>& rhs) noexcept
        {
            components_[0] /= rhs.components_[0];
            components_[1] /= rhs.components_[1];
            components_[2] /= rhs.components_[2];
            return *this;
        }

        template <typename T>
        constexpr T vector3<T>::squared_magnitude() const noexcept
        {
            return vector3<T>::squared_magnitude(*this);
        }

        template <typename T>
        constexpr T vector3<T>::squared_magnitude(const vector3<T>& vector) noexcept
        {
            return dot(vector, vector);
        }

        template <typename T>
        constexpr T vector3<T>::magnitude() const noexcept
        {
            return vector3<T>::magnitude(*this);
        }

        template <typename T>
        constexpr T vector3<T>::magnitude(const vector3<T>& vector) noexcept
        {
            return std::sqrt(squared_magnitude(vector));
        }

        template <typename T>
        constexpr vector3<T> vector3<T>::normalize() const noexcept
        {
            return *this / magnitude();
        }

        template <typename T>
        constexpr vector3<T> vector3<T>::normalize(const vector3<T>& vector) noexcept
        {
            return vector.normalize();
        }

        template <typename T>
        constexpr const vector3<T>& vector3<T>::operator+=(const vector3<T>& v) noexcept
        {
            components_[0] += v.components_[0];
            components_[1] += v.components_[1];
            components_[2] += v.components_[2];
            return *this;
        }

        template <typename T>
        constexpr const vector3<T>& vector3<T>::operator-=(const vector3<T>& v) noexcept
        {
            return *this += -v;
        }

        template <typename T>
        constexpr vector3<T> vector3<T>::zero() noexcept
        {
            return vector3<T>{0};
        }

        template <typename T>
        constexpr vector3<T> vector3<T>::one() noexcept
        {
            return vector3<T>{1};
        }

        template <typename T>
        constexpr vector3<T> vector3<T>::up() noexcept
        {
            return {0, 1, 0};
        }

        template <typename T>
        constexpr vector3<T> vector3<T>::down() noexcept
        {
            return {0, -1, 0};
        }

        template <typename T>
        constexpr vector3<T> vector3<T>::left() noexcept
        {
            return {-1, 0, 0};
        }

        template <typename T>
        constexpr vector3<T> vector3<T>::right() noexcept
        {
            return {1, 0, 0};
        }

        template <typename T>
        constexpr vector3<T> vector3<T>::forward() noexcept
        {
            return {0, 0, 1};
        }

        template <typename T>
        constexpr vector3<T> vector3<T>::backward() noexcept
        {
            return {0, 0, -1};
        }

        template <typename T>
        constexpr vector3<T> vector3<T>::infinity() noexcept
        {
            return vector3<T>{std::numeric_limits<T>::infinity()};
        }

        template <typename T>
        constexpr vector3<T> vector3<T>::negative_infinity() noexcept
        {
            return vector3<T>{-std::numeric_limits<T>::infinity()};
        }
    }

    using vector3 = detail::vector3<float>;
    using vector3_int = detail::vector3<int>;
    using vector3_uint = detail::vector3<unsigned>;
    using point3 = vector3_int;
}
