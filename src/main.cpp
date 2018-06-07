
#include <application/application.hpp>
#include "maths/vector.hpp"
#include "maths/matrix.hpp"

int main(int argc, char* argv[])
{
    constexpr loki::vector3 position { 1, 0, 0 };

    constexpr loki::vector3 position2 { 0, 0, 1 };

    constexpr loki::vector3 position3 = loki::cross(position, position2);

    constexpr loki::matrix4 matrix
    {
        1.0f,  2.0f,  3.0f,  4.0f,
        5.0f,  6.0f,  7.0f,  8.0f,
        9.0f,  10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    };

    loki::application app{ argc, argv };
    return app.run();
}
