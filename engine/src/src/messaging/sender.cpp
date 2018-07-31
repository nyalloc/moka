#include <messaging/sender.hpp>

namespace moka
{
    sender::sender(): _q(nullptr)
    {
    }

    sender::sender(message_queue* q): _q(q)
    {
    }
}
