
#include <SDL.h>
#include <maths/vector2.hpp>

#undef main

#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <future>
#include "logger/logger.hpp"
#include <GL/glew.h>
#include "maths/colour.hpp"
#include "graphics_api_impl.hpp"

namespace moka
{
    enum class key
    {
        none = 0,
        esc,
        enter,
        tab,
        space,
        backspace,
        up,
        down,
        left,
        right,
        insert,
        del,
        home,
        end,
        page_up,
        page_down,
        print,
        plus,
        minus,
        left_bracket,
        right_bracket,
        semicolon,
        quote,
        comma,
        period,
        slash,
        backslash,
        tilde,
        f1,
        f2,
        f3,
        f4,
        f5,
        f6,
        f7,
        f8,
        f9,
        f10,
        f11,
        f12,
        num_pad_0,
        num_pad_1,
        num_pad_2,
        num_pad_3,
        num_pad_4,
        num_pad_5,
        num_pad_6,
        num_pad_7,
        num_pad_8,
        num_pad_9,
        key_0,
        key_1,
        key_2,
        key_3,
        key_4,
        key_5,
        key_6,
        key_7,
        key_8,
        key_9,
        key_a,
        key_b,
        key_c,
        key_d,
        key_e,
        key_f,
        key_g,
        key_h,
        key_i,
        key_j,
        key_k,
        key_l,
        key_m,
        key_n,
        key_o,
        key_p,
        key_q,
        key_r,
        key_s,
        key_t,
        key_u,
        key_v,
        key_w,
        key_x,
        key_y,
        key_z,
        count
    };

    class mouse_motion;
    class key_down;
    class close_queue;
    class create_program;

    // visitor pattern
    class base_dispatcher
    {
    public:
        virtual ~base_dispatcher() = default;
        virtual bool dispatch(const mouse_motion&) { return false; }
        virtual bool dispatch(const key_down&) { return false; }
        virtual bool dispatch(const close_queue&) { return false; }
        virtual bool dispatch(const create_program&) { return false; }
    };

    // templated concrete visitor. Dispatches a specific message type, T.
    // returns false if a different type of message is specified when calling dispatch()
    template<typename T>
    class basic_dispatcher : public base_dispatcher
    {
        std::function<void(const T&)> _func;
    public:
        explicit basic_dispatcher(std::function<void(const T&)> func)
            : _func(std::move(func))
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

        key val;
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

    class create_program : public message_base
    {
        std::function<void(program_handle)> completion_handler;
    public:
        explicit create_program(std::function<void(program_handle)>&& completion_handler)
            : completion_handler(move(completion_handler))
        {}

        void operator()(const program_handle& handle) const
        {
            completion_handler(handle);
        }

        using dispatcher = basic_dispatcher<create_program>;

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
        chained_dispatcher(const chained_dispatcher&) = delete;
        chained_dispatcher& operator=(const chained_dispatcher&) = delete;

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
            return msg->dispatch(v);
        }
    public:
        dispatcher(const dispatcher&) = delete;
        dispatcher& operator=(const dispatcher&) = delete;

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
            else if constexpr(std::is_same<non_blocking, BlockingPolicy>::value)
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

    static std::unordered_map<int, key> sdl_to_moka
    {
        { SDL_SCANCODE_ESCAPE,      key::esc       },
        { SDL_SCANCODE_RETURN,      key::enter     },
        { SDL_SCANCODE_TAB,         key::tab       },
        { SDL_SCANCODE_BACKSPACE,   key::backspace },
        { SDL_SCANCODE_SPACE,       key::space     },
        { SDL_SCANCODE_UP,          key::up        }, 
        { SDL_SCANCODE_DOWN,        key::down      },
        { SDL_SCANCODE_LEFT,        key::left      },
        { SDL_SCANCODE_RIGHT,       key::right     },
        { SDL_SCANCODE_PAGEUP,      key::page_up   },
        { SDL_SCANCODE_PAGEDOWN,    key::page_down },
        { SDL_SCANCODE_HOME,        key::home      },
        { SDL_SCANCODE_END,         key::end       },
        { SDL_SCANCODE_PRINTSCREEN, key::print     },
        { SDL_SCANCODE_KP_PLUS,     key::plus      },
        { SDL_SCANCODE_EQUALS,      key::plus      },
        { SDL_SCANCODE_KP_MINUS,    key::minus     },
        { SDL_SCANCODE_MINUS,       key::minus     },
        { SDL_SCANCODE_GRAVE,       key::tilde     },
        { SDL_SCANCODE_KP_COMMA,    key::comma     },
        { SDL_SCANCODE_COMMA,       key::comma     },
        { SDL_SCANCODE_KP_PERIOD,   key::period    },
        { SDL_SCANCODE_PERIOD,      key::period    },
        { SDL_SCANCODE_SLASH,       key::slash     }
    };

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
                const auto& scancode = e.key.keysym.scancode;
                event.val = sdl_to_moka.at(scancode);
                sender.send(event);
                break;
            }
            default:;
            }
        }
    }

    program_handle create_shader(sender& sender)
    {
        std::cout << "Create program invoked by " << std::this_thread::get_id() << std::endl;

        std::mutex m;
        std::condition_variable cv;
        auto ready = false;

        program_handle handle;

        // register handler to get the program handle from render thread.
        const create_program event([&](program_handle h)
        {
            std::cout << "Program handle callback invoked from " << std::this_thread::get_id() << std::endl;

            handle = h;
            ready = true;
            cv.notify_one();
        });

        // send request to render thread.
        sender.send(event);

        // wait for render thread to complete its work.
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [&] { return ready; });

        std::cout << "Program handle returned from " << std::this_thread::get_id() << std::endl;

        return handle;
    }

    void run(receiver& receiver, sender& sender)
    {
        logger log{ filesystem::current_path(), true };

        auto event_sender = receiver.make_sender();

        run_native_event_loop(event_sender);

        receiver.poll()
        .handle<mouse_motion>([&](const mouse_motion& e)
        {
            log.log(level::debug, e.position);
            log.log(level::debug, e.motion);
        })
        .handle<key_down>([&](const key_down& e)
        {
            switch(e.val) 
            { 
                case key::none: break;
                case key::esc: break;
                case key::enter: 
                {
                    auto program = create_shader(sender);
                    break;
                }
                case key::tab: break;
                case key::space: break;
                case key::backspace: break;
                case key::up: break;
                case key::down: break;
                case key::left: 
                {
                    log.log(level::debug, "left key down");
                    break;
                }
                case key::right: 
                {
                    log.log(level::debug, "right key down");
                    break;
                }
                default: ;
            }
        });
    }
}

// this is all gnarly stuff, raw opengl and SDL will all be hidden in the real deal
int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        std::cout << "REEEE";
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

    auto window = SDL_CreateWindow("SDL Sucks", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL);

    moka::receiver gameplay_receiver;
    moka::receiver graphics_receiver;

    // borrowed from https://skryabiin.wordpress.com/2015/04/25/hello-world/
    // context for graphics thread
    auto sdl_gl_context = SDL_GL_CreateContext(window);
    // context for main thread
    auto sdl_init_context = SDL_GL_CreateContext(window);

    std::thread t([&graphics_receiver, &window, &sdl_gl_context]()
    {
        // steal context
        SDL_GL_MakeCurrent(window, sdl_gl_context);
        auto bg = moka::color::blue();
        
        std::array<std::uint64_t, 2048> buffer;

        while(true)
        {
            // execute commands for creating graphics resources
            graphics_receiver.poll()
            .handle<moka::create_program>([&](const moka::create_program& e)
            {
                // create shader program from args, return it via callback
                moka::program_handle h;
                e(h);
            });

			// sort all render items to minimise state changes
            // http://realtimecollisiondetection.net/blog/?p=86
            std::sort(buffer.begin(), buffer.end());

            // clear the render target 
            // (investigate how these non-draw commands are submitted and sorted)

            glClearColor(bg.r(), bg.g(), bg.b(), bg.a());
            glClear(GL_COLOR_BUFFER_BIT);

			// render sorted lists of items

            for(const auto& renderable : buffer)
            {
                
            }

			// swap the buffer
            SDL_GL_SwapWindow(window);
        }
    });

    while (moka::running)
    {
        auto graphics_sender = graphics_receiver.make_sender();
        run(gameplay_receiver, graphics_sender);
    }

    SDL_Quit();
}
