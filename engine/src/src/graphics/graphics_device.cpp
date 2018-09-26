
#include <graphics/graphics_device.hpp>
#include <graphics/gl_graphics_api.hpp>
#include <graphics/draw_call.hpp>
#include <graphics/draw_call_builder.hpp>
#include <graphics/create_program.hpp>
#include <graphics/create_shader.hpp>
#include <graphics/create_vertex_buffer.hpp>
#include <graphics/create_index_buffer.hpp>
#include <graphics/create_texture.hpp>
#include <graphics/frame.hpp>
#include <application/logger.hpp>
#include <GL/glew.h>

namespace moka
{
    std::unique_ptr<graphics_api> create(
        const graphics_backend backend)
    {
        switch (backend)
        {
        case graphics_backend::direct3d_9:  return std::make_unique<gl_graphics_api>();
        case graphics_backend::direct3d_11: return std::make_unique<gl_graphics_api>();
        case graphics_backend::direct3d_12: return std::make_unique<gl_graphics_api>();
        case graphics_backend::gnm:         return std::make_unique<gl_graphics_api>();
        case graphics_backend::metal:       return std::make_unique<gl_graphics_api>();
        case graphics_backend::opengl_es:   return std::make_unique<gl_graphics_api>();
        case graphics_backend::opengl:      return std::make_unique<gl_graphics_api>();
        case graphics_backend::vulkan:      return std::make_unique<gl_graphics_api>();
        case graphics_backend::null:        return nullptr;
        default:                            return std::make_unique<gl_graphics_api>();
        }
    }

    graphics_device::graphics_device(
		window& window,
        const graphics_backend graphics_backend)
	: window_(window)
	, worker_context_(window_.make_context())
	, main_context_(window_.make_context())
	, graphics_backend_(graphics_backend)
	{
		std::mutex m;
		std::condition_variable cv;
		auto ready = false;

		worker_ = std::thread([&]() 
		{	
			// this is the worker thread that does all rendering
			// this might not be very portable, a lot of literature I've read reccomends using the main thread for this work
			// however this doesn't seem to be possible when using an SDL-based application, as it reserves the main thread for its event loop

			// make worker graphics context current
			window_.set_current_context(worker_context_);

			// initialise renderer backend
			graphics_api_ = create(graphics_backend_);

			ready = true;
			cv.notify_one();

			wait()
			.handle<frame_cmd>([&](const frame_cmd& event)
			{
				graphics_api_->frame();
				window_.swap_buffer();
				event();
			})
			.handle<create_program_cmd>([&](const create_program_cmd& event)
			{
				event(graphics_api_->create_program(
					event.vertex_handle,
					event.fragment_handle));
			})
			.handle<create_shader_cmd>([&](const create_shader_cmd& event)
			{
				event(graphics_api_->create_shader(
					event.type,
					event.source));
			})
			.handle<create_vertex_buffer_cmd>([&](const create_vertex_buffer_cmd& event)
			{
				event(graphics_api_->create_vertex_buffer(
					event.vertices,
					event.size,
					event.layout));
			})
			.handle<create_index_buffer_cmd>([&](const create_index_buffer_cmd& event)
			{
				event(graphics_api_->create_index_buffer(
					event.indices,
					event.size));
			})
			.handle<create_texture_cmd>([&](const create_texture_cmd& event)
			{
				event(graphics_api_->create_texture(event.data));
			});
		});

		// wait for render thread to initialise backend
		std::unique_lock<std::mutex> lk(m);
		cv.wait(lk, [&] { return ready; });
	}

	vertex_buffer_handle graphics_device::create_vertex_buffer(
		const void* vertices, 
		const size_t size, 
		const vertex_layout& layout)
    {
		std::mutex m;
		std::condition_variable cv;
		auto ready = false;

		vertex_buffer_handle handle{};

		// register handler to get the program handle from render thread.
		const create_vertex_buffer_cmd event(vertices, size, layout, [&](vertex_buffer_handle h)
		{
			handle = h;
			ready = true;
			cv.notify_one();
		});

		// send request to render thread.
		send(event);

		// wait for render thread to complete its work.
		std::unique_lock<std::mutex> lk(m);
		cv.wait(lk, [&] { return ready; });

		return handle;
	}

	index_buffer_handle graphics_device::create_index_buffer(const void* indices, const size_t size)
	{
		std::mutex m;
		std::condition_variable cv;
		auto ready = false;

		index_buffer_handle handle{};

		// register handler to get the program handle from render thread.
		const create_index_buffer_cmd event(indices, size, [&](index_buffer_handle h)
		{
			handle = h;
			ready = true;
			cv.notify_one();
		});

		// send request to render thread.
		send(event);

		// wait for render thread to complete its work.
		std::unique_lock<std::mutex> lk(m);
		cv.wait(lk, [&] { return ready; });

		return handle;
	}

    shader_handle graphics_device::create_shader(
        const shader_type type
        , const std::string& source)
    {
		std::mutex m;
		std::condition_variable cv;
		auto ready = false;

		shader_handle handle{};

		//// register handler to get the program handle from render thread.
		const create_shader_cmd event(type, source, [&](shader_handle h)
		{
			handle = h;
			ready = true;
			cv.notify_one();
		});

		//// send request to render thread.
		send(event);

		// wait for render thread to complete its work.
		std::unique_lock<std::mutex> lk(m);
		cv.wait(lk, [&] { return ready; });

		return handle;
	}

	draw_call_builder graphics_device::begin()
	{
		return { *this };
	}

	void graphics_device::submit(
		draw_call&& call)
	{
		graphics_api_->submit(std::move(call));
	}

	void graphics_device::destroy(
        const shader_handle handle) const
    {
    }

	program_handle graphics_device::create_program(
		const shader_handle vertex_handle
		, const shader_handle fragment_handle)
	{
		std::mutex m;
		std::condition_variable cv;
		auto ready = false;

		program_handle handle{};

		// register handler to get the program handle from render thread.
		const create_program_cmd event(vertex_handle, fragment_handle, [&](program_handle h)
		{
			handle = h;
			ready = true;
			cv.notify_one();
		});

		// send request to render thread.
		send(event);

		// wait for render thread to complete its work.
		std::unique_lock<std::mutex> lk(m);
		cv.wait(lk, [&] { return ready; });

		return handle;
	}

	texture_handle graphics_device::create_texture(texture_data& texture_data, bool free_data)
	{
		std::mutex m;
		std::condition_variable cv;
		auto ready = false;

		texture_handle handle{};

		// register handler to get the program handle from render thread.
		const create_texture_cmd event(texture_data, [&](texture_handle h)
		{
			handle = h;
			ready = true;
			cv.notify_one();
		});

		// send request to render thread.
		send(event);

		// wait for render thread to complete its work.
		std::unique_lock<std::mutex> lk(m);
		cv.wait(lk, [&] { return ready; });

		if (free_data)
		{
			unload(texture_data);
		}

		return handle;
	}

    program_handle graphics_device::create_program(
        shader_handle compute_handle)
    {
		return {};
    }

    void graphics_device::destroy(
        program_handle handle)
    {}

	void graphics_device::destroy(
        vertex_buffer_handle handle)
	{}

	void graphics_device::destroy(
		index_buffer_handle handle)
	{}

	void graphics_device::frame()
	{
		std::mutex m;
		std::condition_variable cv;
		auto ready = false;

		const frame_cmd frame([&]()
		{
			ready = true;
			cv.notify_one();
		});

    	send(frame);

		std::unique_lock<std::mutex> lk(m);
		cv.wait(lk, [&] { return ready; });
	}

	uniform_handle graphics_device::create_uniform(const char* name, const uniform_type& type, size_t size)
	{
		return graphics_api_->create_uniform(name, type, size);
	}

	const uniform_data & graphics_device::set_uniform(const uniform_handle& uniform, const void* data)
	{
		return graphics_api_->set_uniform(uniform, data);
	}
}
