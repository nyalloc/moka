
#include <application/application.hpp>
#include <application/mvp/model.hpp>

int main(int argc, char* argv[])
{
    loki::application app{ argc, argv };

    loki::test_class test_1;
    loki::test_class test_2;

    for (size_t i = 0; i < 10000; i++)
    {
        loki::application::post_event(&test_1, test_event{ "Test1: Message " + std::to_string(i) });
        loki::application::post_event(&test_2, test_event{ "Test2: Message " + std::to_string(i) });
    }

    loki::concrete_model model{ app };
    loki::concrete_view view{ app };
    loki::concrete_presenter presenter{ app, model, view };

    return app.run();
}