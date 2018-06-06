
#include <application/application.hpp>
#include "maths/vector.hpp"

int main(int argc, char* argv[])
{
    constexpr loki::vector3 position { 2, 2 };

    constexpr loki::vector3 position2 { 2, 2, 2 };

    constexpr loki::vector3 position3 = cross(position, position2);

    loki::application app{ argc, argv };
    return app.run();
}
