#include <array>
#include <utility>

namespace loki
{
    template<typename T, size_t row, size_t column>
    class matrix
    {
        using column_type = std::array<T, column>;
        using row_type = std::array<T, row>;
        using type = matrix<T, row, column>;
        using transpose_type = matrix<T, column, row>;
        using value_type = T;

        std::array<column_type, column> rows;

    public:
        constexpr column_type& operator[](const size_t index)
        {
            return rows[index];
        }

        constexpr const column_type& operator[](const size_t index) const
        {
            return rows[index];
        }

        constexpr void populate(size_t c, size_t r, T&& first) noexcept
        {
            rows[c][r] = first;
        }

        template<typename... Args>
        constexpr void populate(size_t c, size_t r, T&& first, Args&&... args)  noexcept
        {
            rows[c][r] = first;

            if (c != column - 1)
            {
                populate(++c, r, std::forward<T>(args)...);
            }
            else
            {
                populate(0, ++r, std::forward<T>(args)...);
            }
        }

        template<typename... Args>
        constexpr matrix(Args&&... args) noexcept
            : rows{}
        {
            populate(0, 0, std::forward<T>(args)...);
        }
    };

    template<typename T, size_t row, size_t column, typename T2, size_t row2, size_t column2>
    constexpr auto operator * (const matrix<T, row, column>& lhs, const matrix<T2, row2, column2>& rhs)
    {
        static_assert(column == row2, "Cannot multiply these matrices. Please ensure the number of columns in the first matrix == the number of rows in the second matrix.");
        return lhs;
    }

    using matrix3 = matrix<float, 3, 3>;
    using matrix3x3 = matrix3;
    using mat3 = matrix3;
    using mat3x3 = matrix3;

    using matrix4 = matrix<float, 4, 4>;
    using matrix4x4 = matrix4;
    using mat4 = matrix4;
    using mat4x4 = matrix4;
}