#pragma once

#include <application/window.hpp>
#include <memory>
#include <graphics/api/graphics_api.hpp>
#include <graphics/command/command_list.hpp>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace moka
{
    enum class graphics_backend
    {
        direct3d_9,  
        direct3d_11,
        direct3d_12,
        gnm,         
        metal,       
		opengl_es,   
        opengl,      
        vulkan,     
        null         
    };

	class message
	{
	public:
		virtual void accept(graphics_api& api) = 0;
		virtual ~message() = default;
		message() = default;
		message(const message& rhs) = default;
		message(message&& rhs) = default;
		message& operator=(const message& rhs) = default;
		message& operator=(message&& rhs) = default;
	};

    class graphics_device
    {
		std::unique_ptr<graphics_api> graphics_api_; 

		window& window_;

	public:

        graphics_device(window& window, graphics_backend graphics_backend = graphics_backend::opengl);

        vertex_buffer make_vertex_buffer(const void* vertices, size_t size, vertex_layout&& layout, buffer_usage use);

		index_buffer make_index_buffer(const void* indices, size_t size, index_type type, buffer_usage use);

		shader make_shader(shader_type type, const std::string& source);

        program make_program(shader vertex_handle, shader fragment_handle);

		texture make_texture(void* data, const glm::ivec2& resolution, texture_components components, texture_wrap_mode wrap_mode, bool has_mipmaps, bool free_data = true);

        void destroy(program handle);

		void destroy(shader handle);

        void destroy(vertex_buffer handle);

		void destroy(index_buffer handle);

		void submit(command_list&& command_list, bool sort = true);

		void submit_and_swap(command_list&& command_list, bool sort = true);
    };
}
