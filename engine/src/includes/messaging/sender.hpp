#pragma once

#include <messaging/queue.hpp>

namespace moka
{
    class sender
    {
        message_queue* _q;
    public:
        sender();
        explicit sender(message_queue* q);

        template<typename T>
        void send(T&& msg);
    };

    template <typename T>
    void sender::send(T&& msg)
    {
        if (_q)
        {
            _q->push(std::make_shared<wrapped_message<T>>(std::forward<T>(msg)));
        }
    }
}
