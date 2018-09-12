
#include <graphics/graphics_device.hpp>
#include <graphics/gl_graphics_api.hpp>
#include <graphics/draw_call.hpp>
#include <graphics/create_program.hpp>
#include <graphics/create_shader.hpp>
#include <graphics/create_vertex_buffer.hpp>
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
        case graphics_backend::null:        return std::make_unique<gl_graphics_api>();
        default:                            return std::make_unique<gl_graphics_api>();
        }
    }

	void graphics_device::worker_thread()
	{
		// this is the worker thread that does all rendering
		// this might not be very portable, a lot of literature I've read reccomends using the main thread for this work
		// however this doesn't seem to be possible when using an SDL-based application, as it reserves the main thread for its event loop

		// make worker graphics context current
		window_.set_current_context(worker_context_);

		// initialise renderer backend
		graphics_api_ = create(graphics_backend_);

		draw_call_buffer_pos_ = 0;

		const auto bg = color::cornflower_blue();

		// draw item comparator for sorting
		auto comparator = [](draw_call& a, draw_call& b)
		{
			return a.key > b.key;
		};

		draw_call previous_call = {};

		while (true)
		{
			// execute commands for creating graphics resources
			poll()
			.handle<create_program_cmd>([&](const create_program_cmd& event)
			{
				event(graphics_api_->create_program(event.vertex_handle, event.fragment_handle));
			})
			.handle<create_shader_cmd>([&](const create_shader_cmd& event)
			{
				event(graphics_api_->create_shader(event.type, event.source));
			})
			.handle<create_vertex_buffer_cmd>([&](const create_vertex_buffer_cmd& event)
			{
				event(graphics_api_->create_vertex_buffer(event.vertices, event.layout));
			})
			.handle<frame_cmd>([&](const frame_cmd& event)
			{
				// if draw commands have been sent
				if (draw_call_buffer_pos_ > 0)
				{
					// sort all render items to minimise state changes
					// http://realtimecollisiondetection.net/blog/?p=86
					// only iterate over items actually submitted for rendering (begin to begin + n)
					std::sort(draw_call_buffer_.begin(), draw_call_buffer_.begin() + draw_call_buffer_pos_, comparator);

					// clear the render target 
					graphics_api_->clear_colour(bg);

					for (auto it = draw_call_buffer_.begin(); it != draw_call_buffer_.begin() + draw_call_buffer_pos_; ++it)
					{
						auto& current_call = *it;

						if (previous_call.vertex_buffer.id != current_call.vertex_buffer.id)
						{
							graphics_api_->bind(current_call.vertex_buffer);
						}

						if (previous_call.program.id != current_call.program.id)
						{
							graphics_api_->bind(current_call.program);
						}

						// the buffer allocates a contiguous block of space for each draw call's uniforms
						// we're interested in the block between uniform_start and uniform_end

						uniform_buffer_.set_position(current_call.uniform_start);

						// iterate over every uniform related to this draw call
						while(uniform_buffer_.position() < current_call.uniform_end)
						{
							// the handle is packed into the buffer as a header, allowing us to look up the size & type of the data that follows:

							auto handle = uniform_buffer_.read_uniform_handle(); // get the uniform handle
							auto uniform_data = uniform_data_[handle.id];		 // get the uniform type & count

							// now that we know the size and type of the uniform, we can access the raw data.
							const auto size = get_uniform_size(uniform_data.type, uniform_data.count);
							auto data = uniform_buffer_.read_uniform_body(size);

							//todo: abstact opengl implementation detail to gl_graphics_api

							auto location = glGetUniformLocation(GLuint(current_call.program.id), uniform_data.name.data());

							switch(uniform_data.type) 
							{ 
								case uniform_type::int1: 
								{
									GLuint texture_unit;
									memcpy(&texture_unit, data, size);
									glUniform1uiv(location, uniform_data.count, &texture_unit);
									break;
								}
								case uniform_type::vec3:
								{
									float vec[3];
									memcpy(&vec, data, size);
									glUniform3fv(location, uniform_data.count, vec);
									break;
								}
								case uniform_type::vec4:
								{
									float vec[4];
									memcpy(&vec, data, size);
									glUniform4fv(location, uniform_data.count, vec);
									break;
								}
								case uniform_type::mat3:
								{
									float vec[9];
									memcpy(&vec, data, size);
									glUniformMatrix3fv(location, uniform_data.count, false, vec);
									break;
								}
								case uniform_type::mat4:
								{
									float vec[16];
									memcpy(&vec, data, size);
									glUniformMatrix4fv(location, uniform_data.count, false, vec);
									break;
								}
								default:;
							}
						}

						// if an index buffer is set, draw indexed. Otherwise, draw arrays.
						if (is_handle_valid(current_call.index_buffer))
						{
							graphics_api_->draw_indexed(primitive_type::triangles, current_call.index_count);
						}
						else
						{
							graphics_api_->draw_arrays(primitive_type::triangles, current_call.start_vertex, current_call.vertex_count);
						}

						previous_call = current_call;
					}
				}

				uniform_buffer_.clear();
				draw_call_buffer_pos_ = 0;

				// swap the buffer
				window_.swap_buffer();

				if constexpr (application_traits::is_debug_build)
				{
					graphics_api_->check_errors();
				}

				event(); // tell the main thread we're done rendering
			});
		}
	}

    graphics_device::graphics_device(
		window& window,
        const graphics_backend graphics_backend)
	: window_(window)
	, worker_context_(window_.make_context())
	, main_context_(window_.make_context())
	, graphics_backend_(graphics_backend)
	, worker_([&]() { worker_thread(); })
	, uniform_count_(0)
	{}

	vertex_buffer_handle graphics_device::create_vertex_buffer(
        const memory& vertices
        , const vertex_layout& layout)
    {
		std::mutex m;
		std::condition_variable cv;
		auto ready = false;

		vertex_buffer_handle handle{};

		// register handler to get the program handle from render thread.
		const create_vertex_buffer_cmd event(vertices, layout, [&](vertex_buffer_handle h)
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

	draw_call_builder graphics_device::draw()
	{
		return { *this };
	}

	void graphics_device::submit(
		draw_call&& call)
	{
    	// determine visibility of primitives submition before adding to draw buffer

    	// lock here!
		if (draw_call_buffer_pos_ + 1 < draw_call_buffer_.size())
		{
			draw_call_buffer_[draw_call_buffer_pos_++] = std::move(call);
		}
	}

	void graphics_device::destroy(
        const shader_handle handle) const
    {
        graphics_api_->destroy(handle);
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

	uniform_handle graphics_device::create_uniform(const char* name, const uniform_type& type, const size_t count)
	{
		auto& uniform_data = uniform_data_[uniform_count_];
		uniform_data.type = type;
		uniform_data.name = name;
		uniform_data.count = count;
		return uniform_handle{ static_cast<handle_id>(uniform_count_++) };
	}

	const uniform_data& graphics_device::set_uniform(const uniform_handle& uniform, const void* data)
	{
		auto& uniform_data = uniform_data_[uniform.id];

		// this is the last location the buffer was written to, which is also the start index for our new entry into the buffer.
		// by taking a note of this, we can re-access the data held in the buffer at that index.

    	uniform_data.buffer_start = uniform_buffer_.end();

		uniform_buffer_.set_uniform(uniform, uniform_data.type, data, uniform_data.count);

		uniform_data.buffer_end = uniform_buffer_.end();

		return uniform_data;
	}
}
