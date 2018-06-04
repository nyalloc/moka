
#include <application/application.hpp>

int main(int argc, char* argv[])
{
    std::cout << "Starting main thread: " << std::this_thread::get_id() << std::endl;

    auto test_1 = loki::make_safe_subscriber<loki::test_class>();
    auto test_2 = loki::make_safe_subscriber<loki::test_class>();

    loki::application app{ argc, argv };

    for (size_t i = 0; i < 10000; i++)
    {
        app.post_event(test_1, test_event{ "Test1: Message " + std::to_string(i) });
        app.post_event(test_2, test_event{ "Test2: Message " + std::to_string(i) });
    }

    return app.run();
}