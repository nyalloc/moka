
#include <application/application.hpp>

int main(int argc, char* argv[])
{
    loki::application app{ argc, argv };
    return app.run();
}
