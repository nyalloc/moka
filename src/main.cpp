
#include <application/application.hpp>
#include <application/mvp/model.hpp>

int main(int argc, char* argv[])
{
    // todo: fix lifetime issue. If these objects go out of scope, event_dispatcher will still try to talk to them. Not good!
    // solutions: use shared_ptr. if event_dispatcher owns it, it stays alive.
    // however, shared_ptr is a pain in the arse and is easy to abuse.
    // event_subscribers should somehow subscribe on construction and unsubscribe on destruction, but if the async method
    // is allready operating on a null subscriber this won't fix it. Null checks don't work either as objects can go out of scope after the check.

    loki::test_class test_1;
    loki::test_class test_2;

    loki::application app{ argc, argv };

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