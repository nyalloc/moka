
#include <SDL.h>
#include <maths/vector2.hpp>

#undef main

#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <future>

namespace moka
{
    class mouse_motion;
    class key_down;
    class close_queue;

    // visitor pattern
    class base_dispatcher
    {
    public:
        virtual ~base_dispatcher() = default;
        virtual bool dispatch(const mouse_motion&) { return false; }
        virtual bool dispatch(const key_down&) { return false; }
        virtual bool dispatch(const close_queue&) { return false; }
    };

    // templated concrete visitor. Dispatches a specific message type, T.
    // returns false if a different type of message is specified when calling dispatch()
    template<typename T>
    class basic_dispatcher : public base_dispatcher
    {
        std::function<void(const T&)> _func;
    public:
        explicit basic_dispatcher(std::function<void(const T&)> func)
            : _func(func)
        {}

        bool dispatch(const T& event) override
        {
            _func(event);
            return true;
        }
    };

    class message_base
    {
    public:
        virtual ~message_base() = default;
        virtual bool dispatch(base_dispatcher& visitor) = 0;
    };

    class mouse_motion : public message_base
    {
    public:
        using dispatcher = basic_dispatcher<mouse_motion>;

        bool dispatch(base_dispatcher& visitor) override
        {
            return visitor.dispatch(*this);
        }

        point2 position{ 0, 0 };
        point2 motion{ 0, 0 };
    };

    class key_down : public message_base
    {
    public:
        using dispatcher = basic_dispatcher<key_down>;

        bool dispatch(base_dispatcher& visitor) override
        {
            return visitor.dispatch(*this);
        }
    };

    class close_queue : public message_base
    {
    public:
        class dispatcher : public base_dispatcher
        {
            bool dispatch(const close_queue& event) override
            {
                return true;
            }
        };

        bool dispatch(base_dispatcher& visitor) override
        {
            return visitor.dispatch(*this);
        }
    };

    class queue
    {
        std::mutex _m;
        std::condition_variable _c;
        std::queue<std::shared_ptr<message_base>> _q;
    public:
        template <typename T>
        void push(const T& msg)
        {
            std::lock_guard<std::mutex> lk(_m);
            _q.push(std::make_shared<T>(msg));
            _c.notify_all();
        }

        std::shared_ptr<message_base> wait_and_pop()
        {
            std::unique_lock<std::mutex> lk(_m);
            _c.wait(lk, [&] {return !_q.empty(); });
            auto res = _q.front();
            _q.pop();
            return res;
        }

        std::shared_ptr<message_base> try_and_pop()
        {
            std::unique_lock<std::mutex> lk(_m);
            if (_q.empty())
            {
                return nullptr;
            }
            auto res = _q.front();
            _q.pop();
            return res;
        }
    };

    class sender
    {
        queue* _q;
    public:
        sender()
            : _q(nullptr)
        {}

        explicit sender(queue* q)
            : _q(q)
        {}

        template <typename Message>
        void send(const Message& msg)
        {
            if (_q)
            {
                _q->push(msg);
            }
        }
    };

    class blocking {};
    class non_blocking {};

    template <typename BlockingPolicy
        , typename PreviousDispatcher
        , typename Msg
        , typename Func>
        class chained_dispatcher
    {
        queue* _q;
        PreviousDispatcher* _prev;
        Func _f;
        bool _chained;

        chained_dispatcher(const chained_dispatcher&) = delete;
        chained_dispatcher& operator=(const chained_dispatcher&) = delete;

        template <typename = BlockingPolicy
            , typename Dispatcher
            , typename OtherMsg
            , typename OtherFunc>
            friend class chained_dispatcher;

        void wait_and_dispatch()
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

        void try_and_dispatch()
        {
            const auto msg = _q->try_and_pop();
            if (msg)
            {
                dispatch(msg);
            }
        }

        bool dispatch(const std::shared_ptr<message_base>& msg)
        {
            typename Msg::dispatcher v(_f);
            if (msg->dispatch(v))
            {
                return true;
            }
            return _prev->dispatch(msg);
        }
    public:
        chained_dispatcher(chained_dispatcher&& other) noexcept
            : _q(other._q)
            , _prev(other._prev)
            , _f(std::move(other._f))
            , _chained(other._chained)
        {
            other._chained = true;
        }

        chained_dispatcher(queue* q
            , PreviousDispatcher* prev
            , Func&& f)
            : _q(q)
            , _prev(prev)
            , _f(std::forward<Func>(f))
            , _chained(false)
        {
            prev->_chained = true;
        }

        template <typename OtherMsg, typename OtherFunc>
        chained_dispatcher<BlockingPolicy, chained_dispatcher, OtherMsg, OtherFunc>
            handle(OtherFunc&& of)
        {
            return chained_dispatcher<BlockingPolicy, chained_dispatcher, OtherMsg, OtherFunc>(
                _q, this, std::forward<OtherFunc>(of));
        }

        ~chained_dispatcher() noexcept(false)
        {
            if constexpr(std::is_same<blocking, BlockingPolicy>::value)
            {
                if (!_chained)
                {
                    wait_and_dispatch();
                }
            }
            if constexpr(std::is_same<non_blocking, BlockingPolicy>::value)
            {
                if (!_chained)
                {
                    try_and_dispatch();
                }
            }
        }
    };

    template<typename BlockingPolicy>
    class dispatcher
    {
        queue* _q;
        bool _chained;

        dispatcher(const dispatcher&) = delete;
        dispatcher& operator=(const dispatcher&) = delete;

        template <typename = BlockingPolicy, typename Dispatcher, typename Msg, typename Func>
        friend class chained_dispatcher;

        void wait_and_dispatch() const
        {
            for (;;)
            {
                const auto msg = _q->wait_and_pop();
                dispatch(msg);
            }
        }

        void try_and_dispatch() const
        {
            const auto msg = _q->try_and_pop();
            if (msg)
            {
                dispatch(msg);
            }
        }

        static bool dispatch(const std::shared_ptr<message_base>& msg)
        {
            close_queue::dispatcher v;
            if (msg->dispatch(v))
            {
                return true;
            }

            return false;
        }
    public:
        dispatcher(dispatcher&& other) noexcept
            : _q(other._q)
            , _chained(other._chained)
        {
            other._chained = true;
        }

        explicit dispatcher(queue* q)
            : _q(q)
            , _chained(false)
        {}

        template <typename Message, typename Func>
        chained_dispatcher<BlockingPolicy, dispatcher, Message, Func> handle(Func&& f)
        {
            return chained_dispatcher<BlockingPolicy, dispatcher, Message, Func>(_q, this, std::forward<Func>(f));
        }

        ~dispatcher() noexcept(false)
        {
            if constexpr(std::is_same<blocking, BlockingPolicy>::value)
            {
                if (!_chained)
                {
                    wait_and_dispatch();
                }
            }
            if constexpr(std::is_same<non_blocking, BlockingPolicy>::value)
            {
                if (!_chained)
                {
                    try_and_dispatch();
                }
            }
        }
    };

    class receiver
    {
        queue _q;
    public:
        sender make_sender()
        {
            return sender(&_q);
        }

        dispatcher<blocking> wait()
        {
            return dispatcher<blocking>(&_q);
        }

        dispatcher<non_blocking> poll()
        {
            return dispatcher<non_blocking>(&_q);
        }
    };


    bool running = true;

    void run_native_event_loop(sender& sender)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
            {
                sender.send(close_queue{});
                running = false;
            }
            case SDL_MOUSEMOTION:
            {
                mouse_motion event;
                event.position = point2{ e.motion.x, e.motion.y };
                event.motion = point2{ e.motion.xrel, e.motion.yrel };
                sender.send(event);
                break;
            }
            case SDL_KEYDOWN:
            {
                key_down event;
                sender.send(event);
                break;
            }
            default:;
            }
        }
    }

    void run(receiver& receiver)
    {
        auto sender = receiver.make_sender();

        run_native_event_loop(sender);

        receiver.poll()
        .handle<mouse_motion>([&](const mouse_motion& e)
        {
        })
        .handle<key_down>([&](const key_down& e)
        {
        });
    }
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    auto window = SDL_CreateWindow("moka"
        , SDL_WINDOWPOS_UNDEFINED
        , SDL_WINDOWPOS_UNDEFINED
        , 1280
        , 720
        , SDL_WINDOW_SHOWN
        | SDL_WINDOW_RESIZABLE
    );

    moka::receiver gameplay_receiver;

    while (moka::running)
    {
        run(gameplay_receiver);
    }

    SDL_Quit();
}
