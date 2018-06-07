
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
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    loki::application app{ argc, argv };
    return app.run();
}
