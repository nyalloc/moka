
#include <application/application.hpp>
#include "maths/colour.hpp"
#include "maths/matrix.hpp"

int main(int argc, char* argv[])
{
    constexpr loki::vector4 vector{ 1.0f, 1.0f, 1.0f, 1.0f };
    constexpr loki::color color_1{ 255, 255, 255 };
    constexpr loki::color color_2{ vector };

    constexpr loki::matrix2 mat1
    {
        0, 1,
        1, 0
    };

    constexpr loki::matrix2 mat2 = loki::matrix2::identity();

    loki::application app{ argc, argv };
    return app.run();
}
