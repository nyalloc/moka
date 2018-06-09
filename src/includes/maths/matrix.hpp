#include <array>
#include <utility>

namespace loki
{
    template<typename T, size_t column, size_t row>
    class matrix
    {
        using column_type = std::array<T, row>;
        using row_type = std::array<T, column>;
        using type = matrix<T, column, row>;
        using transpose_type = matrix<T, row, column>;
        using value_type = T;

        std::array<column_type, column> columns;

    public:
        constexpr column_type& operator[](const size_t index)
        {
            return columns[index];
        }

        constexpr const column_type& operator[](const size_t index) const
        {
            return columns[index];
        }

        constexpr void populate(size_t c, size_t r, T&& first) noexcept
        {
            columns[c][r] = first;
        }

        template<typename... Args>
        constexpr void populate(size_t c, size_t r, T&& first, Args&&... args)  noexcept
        {
            columns[c][r] = first;

            if (c != column - 1)
            {
                populate(++c, r, std::forward<T>(args)...);
            }
            else
            {
                populate(0, ++r, std::forward<T>(args)...);
            }
        }

        constexpr matrix() noexcept
            : columns{}
        {}

        template<typename... Args>
        constexpr matrix(T&& first, Args&&... args) noexcept
            : columns{}
        {
            populate(0, 0, std::forward<T>(first), std::forward<T>(args)...);
        }
    };

    template<typename T, size_t m1_columns, size_t m1_rows, size_t m2_columns, size_t m2_rows>
    constexpr matrix<T, m2_columns, m1_rows> operator * (const matrix<T, m1_columns, m1_rows>& lhs, const matrix<T, m2_columns, m2_rows>& rhs)
    {
        static_assert(m1_columns == m2_rows, "Cannot multiply these matrices. Please ensure the number of columns in the first matrix == the number of rows in the second matrix.");

        matrix<T, m2_columns, m1_rows> product;

        for (size_t i = 0; i < m1_rows; ++i)
        {
            for (size_t j = 0; j < m2_columns; ++j)
            {
                product[j][i] = 0;
                for (size_t k = 0; k < m1_columns; ++k)
                {
                    product[j][i] = product[j][i] + (lhs[k][i] * rhs[j][k]);
                }
            }
        }

        return product;
    }

    using matrix3 = matrix<float, 3, 3>;

    using matrix4 = matrix<float, 4, 4>;

    using matrix2x3 = matrix<float, 2, 3>;

    using matrix3x2 = matrix<float, 3, 2>;

    //constexpr matrix3 operator * (const matrix3& lhs, const matrix3& rhs)
    //{
    //    return matrix3
    //    {
    //        lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0] + lhs[0][2] * rhs[2][0] + lhs[0][3] * rhs[3][0],
    //        lhs[0][0] * rhs[0][1] + lhs[0][1] * rhs[1][1] + lhs[0][2] * rhs[2][1] + lhs[0][3] * rhs[3][1],
    //    };
    //}
}