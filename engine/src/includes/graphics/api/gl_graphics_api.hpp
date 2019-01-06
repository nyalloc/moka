#pragma once

#include <graphics/api/graphics_api.hpp>
#include <application/logger.hpp>
#include <application/window.hpp>
#include <GL/glew.h>
#include <graphics/material/material.hpp>
#include <graphics/buffer/vertex_buffer.hpp>
#include <graphics/buffer/index_buffer.hpp>
#include <graphics/shader.hpp>
#include <graphics/program.hpp>
#include <graphics/command/draw_command.hpp>

namespace moka
{
	struct texture_data
	{
		texture_wrap_mode wrap_mode;
		bool has_mipmaps;
		glm::ivec2 resolution;
	};

	struct program_data
	{
	};

	struct shader_data
	{
		shader_type type;
	};

	struct vertex_buffer_data
	{
		buffer_usage buffer_use = buffer_usage::static_draw;
		size_t size{};
		vertex_layout layout;
	};

	struct index_buffer_data
	{
		buffer_usage buffer_use = buffer_usage::static_draw;
		size_t size{};
		index_type type = index_type::uint16;
	};

    /**
     * \brief Convert the interface of OpenGL into the moka rendring API.
     * Allows moka rendering functionality to work at a higher level without being coupled to an OpenGL backend.
     */
    class gl_graphics_api : public graphics_api
    {
		window& window_;

        static logger log_;

		GLuint vao_ = 0;

		static void GLAPIENTRY
			message_callback(GLenum source,
				GLenum type,
				GLuint id,
				GLenum severity,
				GLsizei length,
				const GLchar* message,
				const void* userParam);

		std::unordered_map<uint16_t, vertex_buffer_data> vertex_buffer_data_;
		std::unordered_map<uint16_t, index_buffer_data> index_buffer_data_;

		draw_command previous_command_;
    public:
        gl_graphics_api(window& window);

		gl_graphics_api(const gl_graphics_api& gl_graphics_api) = delete;
		gl_graphics_api(gl_graphics_api&& gl_graphics_api) = delete;
		gl_graphics_api& operator = (const gl_graphics_api& gl_graphics_api) = delete;
		gl_graphics_api& operator = (gl_graphics_api&& gl_graphics_api) = delete;

		virtual ~gl_graphics_api();

		program make_program(const shader& vertex_handle, const shader& fragment_handle) override;
		shader make_shader(shader_type type, const std::string& source) override;
	    vertex_buffer make_vertex_buffer(const void* vertices, size_t size, vertex_layout&& layout, buffer_usage use) override;
		index_buffer make_index_buffer(const void* indices, size_t size, index_type type, buffer_usage use) override;
		texture make_texture(const void* pixels, const glm::ivec2& resolution, texture_components components, texture_wrap_mode wrap_mode, bool has_mipmaps) override;

		void submit(command_list&& commands) override;
		void submit_and_swap(command_list&& commands) override;

		void visit(clear_command& cmd) override;
		void visit(draw_command& cmd) override;
		void visit(viewport_command& cmd) override;
		void visit(scissor_command& cmd) override;
		void visit(fill_vertex_buffer_command& cmd) override;
		void visit(fill_index_buffer_command& cmd) override;
	};
}
