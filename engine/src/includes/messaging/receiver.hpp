#pragma once

#include <messaging/queue.hpp>
#include <messaging/sender.hpp>
#include <messaging/dispatcher.hpp>

namespace moka
{
    class receiver
    {
        message_queue _q;
    public:
        operator sender();

        dispatcher wait();
    };
}
