#pragma once

#include <messaging/queue.hpp>

namespace moka
{
    class close_queue {};

    class dispatcher
    {
        message_queue* _q;
        bool _is_chained;

        dispatcher(const dispatcher& rhs) = delete;
        dispatcher& operator = (const dispatcher& rhs) = delete;

        template<typename Dispatcher, typename Msg, typename Func>
        friend class chained_dispatcher;

        void wait_and_dispatch() const;

        bool dispatch(const std::shared_ptr<message_base>& msg) const;
    public:
        dispatcher(dispatcher&& rhs) noexcept;

        explicit dispatcher(message_queue* q);

        template<typename Dispatcher, typename Msg, typename Func>
        chained_dispatcher<Dispatcher, Msg, Func> handle(Func&& function)
        {
            return chained_dispatcher<Dispatcher, Msg, Func>(_q, this, std::forward<Func>(function));
        }

        ~dispatcher() noexcept(false);
    };
}