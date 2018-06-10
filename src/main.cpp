
#include <application/application.hpp>
#include "maths/colour.hpp"

int main(int argc, char* argv[])
{
    constexpr loki::vector3 vector{ 255, 255, 255 };
    constexpr loki::color color_1{ 255, 255, 255 };
    constexpr loki::color color_2{ vector };

    loki::application app{ argc, argv };
    return app.run();
}
