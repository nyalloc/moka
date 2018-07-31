#pragma once

#include <messaging/queue.hpp>

namespace moka
{
    template<typename PreviousDispatcher, typename Message, typename Func>
    class chained_dispatcher
    {
        message_queue* _q;
        PreviousDispatcher* _previous;
        Func _func;
        bool _is_chained;

        chained_dispatcher(const chained_dispatcher& rhs) = delete;
        chained_dispatcher& operator = (const chained_dispatcher& rhs) = delete;

        template<typename Dispatcher, typename OtherMessage, typename OtherFunction>
        friend class chained_dispatcher;

        void wait_and_dispatch() const;

        bool dispatch(const std::shared_ptr<message_base>& msg) const;

    public:
        chained_dispatcher(chained_dispatcher&& rhs) noexcept;

        explicit chained_dispatcher(message_queue* q, PreviousDispatcher* previous, Func&& function);

        template<typename OtherMessage, typename OtherFunction>
        chained_dispatcher<chained_dispatcher, OtherMessage, OtherFunction> handle(Func&& f);

        ~chained_dispatcher() noexcept(false);
    };

    template <typename PreviousDispatcher, typename Message, typename Func>
    void chained_dispatcher<PreviousDispatcher, Message, Func>::wait_and_dispatch() const
    {
        for (;;)
        {
            const auto msg = _q->wait_and_pop();
            if (dispatch(msg))
            {
                break;
            }
        }
    }

    template <typename PreviousDispatcher, typename Message, typename Func>
    bool chained_dispatcher<PreviousDispatcher, Message, Func>::dispatch(
        const std::shared_ptr<message_base>& msg) const
    {
        if (auto m = std::dynamic_pointer_cast<wrapped_message<Message>>(msg))
        {
            _func(m->contents);
            return true;
        }
        return _previous->dispatch(msg);
    }

    template <typename PreviousDispatcher, typename Message, typename Func>
    chained_dispatcher<PreviousDispatcher, Message, Func>::chained_dispatcher(chained_dispatcher&& rhs) noexcept:
        _q(rhs._q),
        _previous(rhs._previous),
        _func(std::move(rhs._func)),
        _is_chained(rhs._is_chained)
    {
        rhs._is_chained = true;
    }

    template <typename PreviousDispatcher, typename Message, typename Func>
    chained_dispatcher<PreviousDispatcher, Message, Func>::chained_dispatcher(
        message_queue* q,
        PreviousDispatcher* previous,
        Func&& function)
        : _q(q),
        _previous(previous),
        _func(std::forward<Func>(function)),
        _is_chained(false)
    {
        previous->_is_chained = true;
    }

    template <typename PreviousDispatcher, typename Message, typename Func>
    template <typename OtherMessage, typename OtherFunction>
    chained_dispatcher<chained_dispatcher<PreviousDispatcher, Message, Func>, OtherMessage, OtherFunction>
    chained_dispatcher<PreviousDispatcher, Message, Func>::handle(Func&& f)
    {
        return chained_dispatcher<chained_dispatcher, OtherMessage, OtherFunction>(
            _q, this, std::forward<OtherFunction>(f));
    }

    template <typename PreviousDispatcher, typename Message, typename Func>
    chained_dispatcher<PreviousDispatcher, Message, Func>::~chained_dispatcher() noexcept(false)
    {
        if (!_is_chained)
        {
            wait_and_dispatch();
        }
    }
}
