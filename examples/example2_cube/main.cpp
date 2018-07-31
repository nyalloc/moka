
#include <SDL.h>
#include "maths/vector2.hpp"

#undef main

#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <iostream>
#include <future>

namespace messaging 
{

    struct message_base 
    {
        virtual ~message_base() 
        {
        }
    };

    template <typename Msg>
    struct wrapped_message : message_base 
    {
        Msg contents;

        explicit wrapped_message(Msg const &contents_) :
            contents(contents_) 
        {
        }
    };

    class queue 
    {
        std::mutex m;
        std::condition_variable c;
        std::queue<std::shared_ptr<message_base>> q;
    public:
        template <typename T>
        void push(T const &msg) 
        {
            std::lock_guard<std::mutex> lk(m);
            q.push(std::make_shared<wrapped_message<T>>(msg));
            c.notify_all();
        }

        std::shared_ptr<message_base> wait_and_pop() 
        {
            std::unique_lock<std::mutex> lk(m);
            c.wait(lk, [&] {return !q.empty(); });
            auto res = q.front();
            q.pop();
            return res;
        }

        std::shared_ptr<message_base> try_and_pop()
        {
            std::unique_lock<std::mutex> lk(m);
            if(q.empty())
            {
                return nullptr;
            }
            auto res = q.front();
            q.pop();
            return res;
        }
    };

    class sender 
    {
        queue *q;
    public:
        sender() :
            q(nullptr) 
        {
        }

        explicit sender(queue *q_) :
            q(q_) 
        {
        }

        template <typename Message>
        void send(Message const &msg) 
        {
            if (q) 
            {
                q->push(msg);
            }
        }
    };

    class blocking {};
    class non_blocking {};

    template <typename BlockingPolicy, typename PreviousDispatcher, typename Msg, typename Func>
    class chained_dispatcher 
    {
        queue *q;
        PreviousDispatcher *prev;
        Func f;
        bool chained;

        chained_dispatcher(chained_dispatcher const &) = delete;
        chained_dispatcher& operator=(chained_dispatcher const &) = delete;

        template <typename = BlockingPolicy, typename Dispatcher, typename OtherMsg, typename OtherFunc>
        friend class chained_dispatcher;

        void wait_and_dispatch() 
        {
            for (;;) 
            {
                const auto msg = q->wait_and_pop();
                if (dispatch(msg))
                {
                    break;
                }
            }
        }

        void try_and_dispatch()
        {
            const auto msg = q->try_and_pop();
            if (msg)
            {
                dispatch(msg);
            }
        }

        bool dispatch(std::shared_ptr<message_base> const &msg) 
        {
            if (wrapped_message<Msg>* wrapper = dynamic_cast<wrapped_message<Msg>*>(msg.get())) 
            {
                f(wrapper->contents);
                return true;
            }
            return prev->dispatch(msg);
        }
    public:
        chained_dispatcher(chained_dispatcher &&other) noexcept :
            q(other.q), prev(other.prev), f(std::move(other.f)), chained(other.chained)
        {
            other.chained = true;
        }

        chained_dispatcher(queue *q_, PreviousDispatcher *prev_, Func &&f_) :
            q(q_), prev(prev_), f(std::forward<Func>(f_)), chained(false) 
        {
            prev_->chained = true;
        }

        template <typename OtherMsg, typename OtherFunc>
        chained_dispatcher<BlockingPolicy, chained_dispatcher, OtherMsg, OtherFunc>
            handle(OtherFunc &&of) 
        {
            return chained_dispatcher<BlockingPolicy, chained_dispatcher, OtherMsg, OtherFunc>(q, this, std::forward<OtherFunc>(of));
        }

        ~chained_dispatcher() noexcept(false)
        {
            if constexpr(std::is_same<blocking, BlockingPolicy>::value)
            {
                if (!chained)
                {
                    wait_and_dispatch();
                }
            }
            if constexpr(std::is_same<non_blocking, BlockingPolicy>::value)
            {
                if (!chained)
                {
                    try_and_dispatch();
                }
            }
        }
    };

    class close_queue
    {
    };

    template<typename BlockingPolicy>
    class dispatcher 
    {
        queue *q;
        bool chained;

        dispatcher(dispatcher const &) = delete;
        dispatcher& operator=(dispatcher const &) = delete;

        template <typename = BlockingPolicy, typename Dispatcher, typename Msg, typename Func>
        friend class chained_dispatcher;

        void wait_and_dispatch() const
        {
            for (;;) 
            {
                const auto msg = q->wait_and_pop();
                dispatch(msg);
            }
        }

        void try_and_dispatch() const
        {
            const auto msg = q->try_and_pop();
            if (msg)
            {
                dispatch(msg);
            }
        }

        static bool dispatch(std::shared_ptr<message_base> const &msg) 
        {
            if (dynamic_cast<wrapped_message<close_queue>*>(msg.get())) 
            {
                return true;
            }
            return false;
        }
    public:
        dispatcher(dispatcher &&other) noexcept :
            q(other.q), chained(other.chained) 
        {
            other.chained = true;
        }

        explicit dispatcher(queue *q_) :
            q(q_), chained(false) 
        {
        }

        template <typename Message, typename Func>
        chained_dispatcher<BlockingPolicy, dispatcher, Message, Func> handle(Func &&f) 
        {
            return chained_dispatcher<BlockingPolicy, dispatcher, Message, Func>(q, this, std::forward<Func>(f));
        }

        ~dispatcher() noexcept(false)
        {
            if constexpr(std::is_same<blocking, BlockingPolicy>::value)
            {
                if (!chained)
                {
                    wait_and_dispatch();
                }
            }
            if constexpr(std::is_same<non_blocking, BlockingPolicy>::value)
            {
                if (!chained)
                {
                    try_and_dispatch();
                }
            }
        }
    };

    class receiver 
    {
        queue q;
    public:
        sender make_sender()
        {
            return sender(&q);
        }

        dispatcher<blocking> wait() 
        {
            return dispatcher<blocking>(&q);
        }

        dispatcher<non_blocking> poll()
        {
            return dispatcher<non_blocking>(&q);
        }
    };
}

struct mouse_motion
{
    moka::point2 position{ 0, 0 };
    moka::point2 motion{ 0, 0 };
};

struct key_down
{
    
};

class test_application
{
    
};

bool running = true;

void run_native_event_loop(messaging::sender& sender)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            {
                sender.send(messaging::close_queue{});
                running = false;
            }
        case SDL_MOUSEMOTION:
            {
                mouse_motion event;
                event.position = moka::point2{ e.motion.x, e.motion.y };
                event.motion = moka::point2{ e.motion.xrel, e.motion.yrel };
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

void run(messaging::receiver& receiver)
{
    auto sender = receiver.make_sender();

    run_native_event_loop(sender);

    receiver.poll()
    .handle<mouse_motion>([&](const mouse_motion& e)
    {
        std::cout << "Mouse motion event. Motion:" << e.motion << ", Position: " << e.position << std::endl;
    })
    .handle<key_down>([&](const key_down& e)
    {
        std::cout << "Key down" << std::endl;
    });

    std::cout << "Rendering..." << std::endl;
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

    messaging::receiver gameplay_receiver;

    while (running)
    {
        run(gameplay_receiver);
    }

    SDL_Quit();
}
