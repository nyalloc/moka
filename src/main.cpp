
#include <application/application.hpp>
#include "application/mvp/model.hpp"

int main(int argc, char* argv[])
{
    loki::application app{ argc, argv };

    loki::concrete_model model{ app };
    loki::concrete_view view{ app };
    loki::concrete_presenter presenter{ app, model, view };

    return app.run();
}