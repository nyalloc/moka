#include <array>
#include <utility>

namespace loki
{
    template<typename T>
    class matrix2
    {
        std::array<std::array<T, 2>, 2> components;
    };

    template<typename T>
    class matrix3
    {
        std::array<std::array<T, 3> 3> components;
    };

    template<typename T>
    class matrix4
    {
        std::array<std::array<T, 4>, 4> components;
    };
}