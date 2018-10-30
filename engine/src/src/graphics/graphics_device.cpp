
#include <graphics/graphics_device.hpp>
#include <graphics/gl_graphics_api.hpp>
#include <graphics/messages.hpp>
#include <application/window.hpp>
#include "application/profile.hpp"

namespace moka
{
    std::unique_ptr<graphics_api> create(window& window, const graphics_backend graphics_backend)
    {
		// right now we only have an OpenGL backend!
        switch (graphics_backend)
        {
        case graphics_backend::direct3d_9:  return std::make_unique<gl_graphics_api>(window);
        case graphics_backend::direct3d_11: return std::make_unique<gl_graphics_api>(window);
        case graphics_backend::direct3d_12: return std::make_unique<gl_graphics_api>(window);
        case graphics_backend::gnm:         return std::make_unique<gl_graphics_api>(window);
        case graphics_backend::metal:       return std::make_unique<gl_graphics_api>(window);
        case graphics_backend::opengl_es:   return std::make_unique<gl_graphics_api>(window);
        case graphics_backend::opengl:      return std::make_unique<gl_graphics_api>(window);
        case graphics_backend::vulkan:      return std::make_unique<gl_graphics_api>(window);
        case graphics_backend::null:        return nullptr;
        default:                            return std::make_unique<gl_graphics_api>(window);
        }
    }

    graphics_device::graphics_device(window& window, const graphics_backend graphics_backend)
		: window_(window)
	{
		bool ready = false;

		auto worker_context = window.make_context();
		auto main_context = window.make_context();

		std::condition_variable condition_variable_;

		worker_ = std::thread([&]()
		{
			window.set_current_context(worker_context);

			graphics_api_ = create(window, graphics_backend);

			{
				std::lock_guard<std::mutex> lock(mutex_);
				ready = true;
			}

			condition_variable_.notify_one();

			// send messages to the render backend
			while (true)
			{
				if (!graphics_api_)
				{
					std::cout << "Error: graphics backend set to null" << std::endl;
					return;
				}

				auto val = messages_.pop();  // wait for message to be pushed

				if (!val)
				{
					std::cout << "Err" << std::endl;
					continue;
				}

				val->accept(*graphics_api_); // dispatch message
			}
		});

		// wait for worker thread to initialise the render backend before returning control to the application

		std::unique_lock<std::mutex> lock(mutex_);

		condition_variable_.wait(lock, [&]
		{ 
			return ready; 
		});
	}

	void graphics_device::submit(command_list&& command_list, bool sort)
	{
		if (sort && !command_list.is_sorted())
		{
			command_list.sort();
		}

		messages_.push(std::make_unique<submit_commands>(std::move(command_list), false, []{}));
	}

	void graphics_device::submit_and_swap(command_list&& command_list, bool sort)
	{
		std::condition_variable condition_variable_;

		if (sort && !command_list.is_sorted())
		{
			command_list.sort();
		}

		float ready = false;

		messages_.push(std::make_unique<submit_commands>(std::move(command_list), true, [&]()
		{
			{
				std::lock_guard<std::mutex> lock(mutex_);

				ready = true;
			}
			condition_variable_.notify_one();
		}));

		// wait for render thread to complete its work.
		std::unique_lock<std::mutex> lock(mutex_);

		condition_variable_.wait(lock, [&]
		{
			return ready;
		});
	}

	vertex_buffer graphics_device::make_vertex_buffer(const void* vertices, const size_t size, vertex_layout&& layout, const buffer_usage use)
    {
		std::condition_variable condition_variable_;

		vertex_buffer response;
		bool ready = false;

		// message the render thread and ask for a vertex buffer. Wait here until we get a response.

		messages_.push(std::make_unique<moka::make_vertex_buffer>(vertices, size, std::move(layout), use, [&](vertex_buffer&& result)
		{
			{
				std::lock_guard<std::mutex> lock(mutex_);

				response = result;

				ready = true;
			}
			condition_variable_.notify_one();
		}));

		// wait for render thread to complete its work.

		std::unique_lock<std::mutex> lock(mutex_);

		condition_variable_.wait(lock, [&]
		{
			return ready;
		});

		return response;
	}

	index_buffer graphics_device::make_index_buffer(const void* indices, const size_t size, const index_type type, const buffer_usage use)
	{
		std::condition_variable condition_variable_;

		index_buffer response;
		bool ready = false;

		// message the render thread and ask for an index buffer. Wait here until we get a response.

		messages_.push(std::make_unique<moka::make_index_buffer>(indices, size, type, use, [&](index_buffer&& result)
		{
			{
				std::lock_guard<std::mutex> lock(mutex_);

				response = result;

				ready = true;
			}
			condition_variable_.notify_one();
		}));

		// wait for render thread to complete its work.

		std::unique_lock<std::mutex> lock(mutex_);

		condition_variable_.wait(lock, [&]
		{
			return ready;
		});

		return response;
	}

    shader graphics_device::make_shader(const shader_type type, const std::string& source)
    {
		std::condition_variable condition_variable_;

		shader response{};
		bool ready = false;

		// message the render thread and ask for an index buffer. Wait here until we get a response.

		messages_.push(std::make_unique<moka::make_shader>(type, source, [&](shader&& result)
		{
			{
				std::lock_guard<std::mutex> lock(mutex_);

				response = result;

				ready = true;
			}
			condition_variable_.notify_one();
		}));

		// wait for render thread to complete its work.

		std::unique_lock<std::mutex> lock(mutex_);

		condition_variable_.wait(lock, [&]
		{
			return ready;
		});

		return response;
	}

	program graphics_device::make_program(const shader vertex_handle, const shader fragment_handle)
	{
		std::condition_variable condition_variable_;

		program response{};
		bool ready = false;

		// message the render thread and ask for an index buffer. Wait here until we get a response.

		messages_.push(std::make_unique<moka::make_program>(vertex_handle, fragment_handle, [&](program&& result)
		{
			{
				std::lock_guard<std::mutex> lock(mutex_);

				response = result;

				ready = true;
			}
			condition_variable_.notify_one();
		}));

		// wait for render thread to complete its work.

		std::unique_lock<std::mutex> lock(mutex_);

		condition_variable_.wait(lock, [&]
		{
			return ready;
		});

		return response;
	}

	texture graphics_device::make_texture(void* data, const glm::ivec2& resolution, texture_components components, texture_wrap_mode wrap_mode, bool has_mipmaps, bool free_data)
	{
		std::condition_variable condition_variable_;

		texture response{};
		bool ready = false;

		// message the render thread and ask for an index buffer. Wait here until we get a response.

		messages_.push(std::make_unique<moka::make_texture>(data, resolution, components, wrap_mode, has_mipmaps, [&](texture&& result)
		{
			{
				std::lock_guard<std::mutex> lock(mutex_);

				response = result;

				ready = true;
			}
			condition_variable_.notify_one();
		}));

		// wait for render thread to complete its work.

		std::unique_lock<std::mutex> lock(mutex_);

		condition_variable_.wait(lock, [&]
		{
			return ready;
		});

		if (free_data)
		{
			free_texture(data);
		}

		return response;
	}

    void graphics_device::destroy(program handle)
    {}

	void graphics_device::destroy(vertex_buffer handle)
	{}

	void graphics_device::destroy(index_buffer handle)
	{}

	void graphics_device::destroy(const shader handle)
	{}
}
