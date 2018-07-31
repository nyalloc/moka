
#include <SDL.h>
#include <messaging/sender.hpp>
#include <messaging/dispatcher.hpp>
#include "maths/vector2.hpp"
#include "messaging/receiver.hpp"

#undef main

struct mouse_motion
{
    moka::point2 positon{ 0, 0 };
};

class test_application
{
    
};

void run_native_event_loop(moka::sender& sender)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
        {
            sender.send(moka::close_queue{});
            break;
        }
        case SDL_MOUSEMOTION:
        {
            mouse_motion motion;
            motion.positon = moka::point2{ event.motion.x, event.motion.y };
            sender.send(motion);
            break;
        }
        default:;
        }
    }
}

int main()
{
    moka::receiver receiver;
    moka::sender sender = receiver;

    std::thread thread{ [&sender]()
    {
        run_native_event_loop(sender);
    }};

    receiver.wait()
    .handle<mouse_motion>([&](const mouse_motion& event)
    {
        
    });

    thread.join();
}
