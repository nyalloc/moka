
#include <application/application.hpp>
#include "maths/vector.hpp"
#include "maths/matrix.hpp"

int main(int argc, char* argv[])
{
    constexpr loki::vector3 position { 1, 0, 0 };

    constexpr loki::vector3 position2 { 0, 0, 1 };

    constexpr loki::vector3 position3 = loki::cross(position, position2);

    loki::matrix2x3 mat2x3
    {
        2.0f, 4.0f,
        6.0f, 3.0f,
        1.0f, 2.0f,
    };

    loki::matrix3x2 mat3x2
    {
        3.0f, 4.0f, 1.0f,
        3.0f, 3.0f, 8.0f
    };

    auto result = mat2x3 * mat3x2;

    loki::application app{ argc, argv };
    return app.run();
}
