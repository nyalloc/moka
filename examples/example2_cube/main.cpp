
#include <SDL.h>
#include <maths/vector2.hpp>

#undef main

#include <mutex>
#include <condition_variable>
#include <queue>
#include <future>
#include "logger/logger.hpp"
#include <GL/glew.h>
#include "maths/colour.hpp"
#include <graphics/graphics_api.hpp>
#include <graphics/graphics_device.hpp>
#include "maths/matrix.hpp"
#include <messaging/message_base.hpp>
#include <messaging/base_dispatcher.hpp>
#include <messaging/dispatcher.hpp>
#include <messaging/close_queue.hpp>
#include <messaging/basic_dispatcher.hpp>
#include <messaging/sender.hpp>
#include <messaging/receiver.hpp>

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

	class sort_key
    {
		uint64_t _key = 0;

	public:
		sort_key() = default;

		explicit sort_key(const program_handle program)
			: _key
    	    (
				static_cast<uint64_t>(program.id)
			)
		{}

		program_handle program_handle() const
		{
			return { static_cast<uint16_t>(_key) };
		}

		bool operator > (const sort_key& rhs) const
		{
			return _key > rhs._key;
		}

		bool operator < (const sort_key& rhs) const
		{
			return _key < rhs._key;
		}

		bool operator <= (const sort_key& rhs) const
		{
			return _key <= rhs._key;
		}

		bool operator >= (const sort_key& rhs) const
		{
			return _key >= rhs._key;
		}

		bool operator == (const sort_key& rhs) const
		{
			return _key == rhs._key;
		}
    };

	class draw_call_builder;

	class draw_call
	{
	public:
		program_handle program;
		texture_handle texture;
		vertex_buffer_handle vertex_buffer;
		uint64_t state;
		sort_key key;

		static draw_call_builder builder();
	};

	class draw_call_builder
	{
		draw_call call;
	public:
		template<typename T>
		draw_call_builder & set_uniform(const uniform_handle uniform_handle, const T& value)
		{
			return *this;
		}

		draw_call_builder& set_texture(const size_t texture_unit, const texture_handle texture)
		{
			return *this;
		}

		draw_call_builder& set_face_culling(const face_culling culling)
		{
			return *this;
		}

		draw_call_builder& set_texture(const texture_handle texture)
		{
			return *this;
		}

		draw_call_builder& set_vertex_buffer(const vertex_buffer_handle vertex_buffer)
		{
			return *this;
		}

		draw_call_builder& set_index_buffer(const index_buffer_handle index_buffer)
		{
			return *this;
		}

		draw_call_builder& set_program(const program_handle program)
		{
			return *this;
		}

		draw_call build() const
		{
			return call;
		}
	};

	draw_call_builder draw_call::builder()
	{
		return {};
	}

	std::array<draw_call, 2048> buffer;
	size_t buffer_n = 0;

	void submit(draw_call&& call)
	{
		// lock here
		buffer[buffer_n++] = std::move(call);
	}

	void run(receiver& receiver)
	{
		logger log{ filesystem::current_path(), true };

		auto event_sender = receiver.make_sender();

		run_native_event_loop(event_sender);

		const moka::vector3 vector(0, 0, 0);
		const moka::matrix2 matrix(0, 0, 0, 0);
		const moka::colour colour(0, 0, 0, 0);

		const vertex_buffer_handle vertex_buffer{};
		const program_handle program{};
		const face_culling culling{};
		const texture_handle texture1{};
		const texture_handle texture2{};

		const uniform_handle vector_uniform{};
		const uniform_handle color_uniform{};
		const uniform_handle matrix_uniform{};

		// submit a draw call for rendering
		submit(draw_call::builder()
			.set_vertex_buffer(vertex_buffer)
			.set_program(program)
			.set_face_culling(culling)
			.set_texture(0, texture1)
			.set_texture(0, texture2)
			.set_uniform(vector_uniform, vector)
			.set_uniform(color_uniform, matrix)
			.set_uniform(matrix_uniform, colour)
			.build());
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

    // context for graphics thread
    auto sdl_gl_context = SDL_GL_CreateContext(window);
    // context for main thread (needed for some reason...)
    auto sdl_init_context = SDL_GL_CreateContext(window);
	// borrowed from https://skryabiin.wordpress.com/2015/04/25/hello-world/

    std::thread t([&graphics_receiver, &window, &sdl_gl_context]()
    {
		moka::graphics_device graphics;

        // steal context from main thread
        SDL_GL_MakeCurrent(window, sdl_gl_context);

        auto bg = moka::color::cornflower_blue();
        
		// draw item comparator for sorting
		auto comparator = [](moka::draw_call& a, moka::draw_call& b)
		{
			return a.key > b.key;
		};

        while(true)
        {
            // execute commands for creating graphics resources
			// these can be returned to the caller via callback std::function
        	graphics_receiver.poll()
            .handle<moka::create_program>([&](const moka::create_program& e)
            {
                // create shader program from args, return it via callback
                moka::program_handle h;
                e(h);
            });

			if (moka::buffer_n > 0)
			{
				// sort all render items to minimise state changes
				// http://realtimecollisiondetection.net/blog/?p=86
				// only iterate over items actually submitted for rendering (begin to begin + n)
				std::sort(moka::buffer.begin(), moka::buffer.begin() + moka::buffer_n, comparator);

				// clear the render target 
				// (investigate how these non-draw commands are submitted and sorted)
				glClearColor(bg.r(), bg.g(), bg.b(), bg.a());
				glClear(GL_COLOR_BUFFER_BIT);

				for (auto it = moka::buffer.begin(); it != moka::buffer.begin() + moka::buffer_n; ++it)
				{
					const auto& item = *it;

					graphics.draw_indexed(moka::primitive_type::triangles, 0, 3);
				}

				moka::buffer_n = 0;
			}

			// swap the buffer
            SDL_GL_SwapWindow(window);
        }
    });

    while (moka::running)
    {
        auto graphics_sender = graphics_receiver.make_sender();
        run(gameplay_receiver);
    }

    SDL_Quit();
}
