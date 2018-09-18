#pragma once

#include <graphics/graphics_api.hpp>
#include <application/logger.hpp>
#include <GL/glew.h>
#include "draw_call.hpp"
#include "uniform_buffer.hpp"
#include "uniform_buffer.hpp"

namespace moka
{
    /**
     * \brief Convert the interface of OpenGL into the moka rendring API.
     * Allows moka rendering functionality to work at a higher level without being coupled to an OpenGL backend.
     */
    class gl_graphics_api : public graphics_api
    {
		constexpr static size_t max_uniforms = 64;
		constexpr static size_t max_draw_calls = 2048;

        static logger log_;

        std::vector<shader_handle> shaders_;
        std::vector<program_handle> programs_;
		std::vector<vertex_buffer_handle> vertex_buffers_;
		vertex_buffer_handle current_vertex_;
		std::vector<index_buffer_handle> index_buffers_;

		GLuint vao_ = 0; // single VAO for entire project

		draw_call previous_call_ = {};

		std::unordered_map<handle_id, vertex_layout> vertex_layouts_;

		size_t uniform_count_ = 0;
		uniform_buffer uniform_buffer_;
		std::array<uniform_data, max_uniforms> uniform_data_;

		size_t draw_call_buffer_pos_ = 0;
		std::array<draw_call, max_draw_calls> draw_call_buffer_;

		static void GLAPIENTRY
			message_callback(GLenum source,
				GLenum type,
				GLuint id,
				GLenum severity,
				GLsizei length,
				const GLchar* message,
				const void* userParam);
    public:
        gl_graphics_api();
		~gl_graphics_api();

		void frame() override;

		void submit(draw_call&& call) override;

		program_handle create_program(const shader_handle& vertex_handle, const shader_handle& fragment_handle) override;
        shader_handle create_shader(const shader_type type, const std::string& source) override;
	    vertex_buffer_handle create_vertex_buffer(const void* vertices, size_t size, const vertex_layout& decl) override;
		index_buffer_handle create_index_buffer(const void* indices, size_t size) override;
		texture_handle create_texture(const texture_data& data) override;

		uniform_handle create_uniform(const char* name, const uniform_type& type, const size_t count) override;
		const uniform_data& set_uniform(const uniform_handle& uniform, const void* data) override;
	};
}
