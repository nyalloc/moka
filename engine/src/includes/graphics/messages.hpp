#pragma once

#include <graphics/graphics_device.hpp>
#include <graphics/graphics_api.hpp>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <graphics/blocking_queue.hpp>
#include <application/profile.hpp>

namespace moka
{
	// creation of GPU resources or submission of graphics commands is peformed using these messages.
	// similair to a command, a message encapsulates a unit of work to be given to the GPU.
	// however, commands are by design fire & forget, whereas messages are used when synchonisation 
	// between the application & the render thread is necessary.

	template<typename... Args>
	using response_handler = std::function<void(Args...)>;

	class make_vertex_buffer : public message
	{
		const void* data;
		size_t size;
		vertex_layout layout;
		buffer_usage use;
		response_handler<vertex_buffer&&> handler;
	public:

		virtual ~make_vertex_buffer() = default;

		make_vertex_buffer(const void* data, size_t size, vertex_layout&& layout, buffer_usage use, response_handler<vertex_buffer&&>&& handler)
			: data(data), size(size), layout(std::move(layout)), use(use), handler(std::move(handler))
		{}

		void accept(graphics_api& api) override
		{
			handler(api.make_vertex_buffer(data, size, std::move(layout), use));
		}
	};

	class make_index_buffer : public message
	{
		const void* data;
		size_t size;
		response_handler<index_buffer&&> handler;
		index_type type;
		buffer_usage use;
	public:

		virtual ~make_index_buffer() = default;

		make_index_buffer(const void* data, size_t size, index_type type, buffer_usage use, response_handler<index_buffer&&>&& handler)
			: data(data), size(size), type(type), use(use), handler(std::move(handler))
		{}

		void accept(graphics_api& api) override
		{
			handler(api.make_index_buffer(data, size, type, use));
		}
	};

	class make_shader : public message
	{
		shader_type type;
		const std::string& source;
		response_handler<shader&&> handler;

	public:

		virtual ~make_shader() = default;

		make_shader(const shader_type type, const std::string& source, response_handler<shader&&>&& handler)
			: type(type), source(source), handler(std::move(handler))
		{}

		void accept(graphics_api& api) override
		{
			handler(api.make_shader(type, source));
		}
	};

	class make_program : public message
	{
		const shader& fragment;
		const shader& vertex;
		response_handler<program&&> handler;

	public:

		virtual ~make_program() = default;

		make_program(const shader& fragment, const shader& vertex, response_handler<program&&>&& handler)
			: fragment(fragment), vertex(vertex), handler(std::move(handler))
		{}

		void accept(graphics_api& api) override
		{
			handler(api.make_program(vertex, fragment));
		}
	};

	class make_texture : public message
	{
		void* data;
		glm::ivec2 resolution;
		texture_components components;
		texture_wrap_mode wrap_mode;
		bool has_mipmaps;
		response_handler<texture&&> handler;
	public:
		make_texture(
			void* data,
			const glm::ivec2& resolution,
			texture_components components,
			texture_wrap_mode wrap_mode,
			bool has_mipmaps,
			response_handler<texture&&>&& handler)
			: data(data), 
			resolution(resolution),
			components(components),
			wrap_mode(wrap_mode),
			has_mipmaps(has_mipmaps),
			handler(std::move(handler))
		{}

		virtual ~make_texture() = default;

		void accept(graphics_api& api) override
		{
			handler(api.make_texture(data, resolution, components, wrap_mode, has_mipmaps));
		}
	};

	class submit_commands : public message
	{
		command_list commands;
		bool swap = false;
		response_handler<> handler;
	public:
		submit_commands(command_list&& commands, bool swap, response_handler<>&& handler)
			: commands(std::move(commands)), swap(swap), handler(std::move(handler))
		{}

		virtual ~submit_commands() = default;

		void accept(graphics_api& api) override
		{
			auto submit_time = profile<milliseconds>([&]()
			{
				if (swap)
				{
					api.submit_and_swap(std::move(commands));
				}
				else
				{
					api.submit(std::move(commands));
				}
			});

			handler();
		}
	};
}