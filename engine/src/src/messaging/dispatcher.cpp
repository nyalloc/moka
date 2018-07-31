#include <messaging/dispatcher.hpp>

namespace moka
{
    void dispatcher::wait_and_dispatch() const
    {
        for (;;)
        {
            const auto msg = _q->wait_and_pop();
            dispatch(msg);
        }
    }

    dispatcher::dispatcher(dispatcher&& rhs) noexcept: _q(rhs._q), _is_chained(rhs._is_chained)
    {
        rhs._is_chained = true;
    }

    dispatcher::dispatcher(message_queue* q):
        _q(q), _is_chained(false)
    {
    }

    dispatcher::~dispatcher() noexcept(false)
    {
        if (!_is_chained)
        {
            wait_and_dispatch();
        }
    }

    bool dispatcher::dispatch(const std::shared_ptr<message_base>& msg) const
    {
        if (std::dynamic_pointer_cast<wrapped_message<close_queue>>(msg))
        {
            throw close_queue();
        }
        return false;
    }
}
