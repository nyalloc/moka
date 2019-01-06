
#include <string>
#include <application/logger.hpp>
#include <graphics/api/gl_graphics_api.hpp>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <graphics/material/material.hpp>
#include <graphics/command/command_list.hpp>
#include <application/window.hpp>
#include <graphics/buffer/vertex_layout.hpp>

namespace moka
{
	constexpr GLenum moka_to_gl(const polygon_draw_mode type)
	{
		switch (type)
		{
		case polygon_draw_mode::fill: return GL_FILL;
		case polygon_draw_mode::line: return GL_LINE;
		case polygon_draw_mode::points: return GL_POINTS;
		default: return 0;
		}
	}

	constexpr GLenum moka_to_gl(const face type)
	{
		switch(type)
		{
		case face::front: return GL_FRONT;
		case face::back: return GL_BACK;
		case face::front_and_back: return GL_FRONT_AND_BACK;
		default: return 0;
		}
	}

	constexpr GLenum moka_to_gl(const primitive_type type)
	{
		switch (type)
		{
		case primitive_type::lines: return GL_LINES;
		case primitive_type::line_strip: return GL_LINE_STRIP;
		case primitive_type::triangles: return GL_TRIANGLES;
		case primitive_type::triangle_strip: return GL_TRIANGLE_STRIP;
		default: return 0;
		}
	}

	constexpr GLenum moka_to_gl(const index_type type)
	{
		switch (type)
		{
		case index_type::int8: return GL_BYTE;
		case index_type::uint8: return GL_UNSIGNED_BYTE;
		case index_type::int16: return GL_SHORT;
		case index_type::uint16: return GL_UNSIGNED_SHORT;
		case index_type::int32: return GL_INT;
		case index_type::uint32: return GL_UNSIGNED_INT;
		case index_type::float32: return GL_FLOAT;
		default: return 0;
		}
	}

	constexpr GLenum moka_to_gl(const texture_components type)
	{
		switch (type)
		{
		case texture_components::rgb: return GL_RGB;
		case texture_components::rgb_alpha: return GL_RGBA;
		case texture_components::grey_alpha: return GL_RGBA; //todo: this is probably wrong
		case texture_components::grey: return GL_RGB;		 //todo: ditto
		default: return 0;
		}
	}


	constexpr GLenum moka_to_gl(const attribute_type type)
	{
		switch (type)
		{
		case attribute_type::boolean: return GL_BOOL;
		case attribute_type::int8: return GL_BYTE;
		case attribute_type::int16: return GL_SHORT;
		case attribute_type::int32: return GL_INT;
		case attribute_type::uint8: return GL_UNSIGNED_BYTE;
		case attribute_type::uint16: return GL_UNSIGNED_SHORT;
		case attribute_type::uint32: return GL_UNSIGNED_INT;
		case attribute_type::float16: return GL_SHORT;
		case attribute_type::float32: return GL_FLOAT;
		case attribute_type::float64: return GL_DOUBLE;
		default: return 0;
		}
	}

	constexpr GLenum moka_to_gl(const shader_type type)
	{
		switch (type)
		{
		case shader_type::vertex:
			return GL_VERTEX_SHADER;
		case shader_type::fragment:
			return GL_FRAGMENT_SHADER;
		case shader_type::compute:
			return GL_COMPUTE_SHADER;
		default: return 0;
		}
	}

	constexpr GLenum moka_to_gl(const blend_equation type)
	{
		switch(type) 
		{ 
			case blend_equation::func_add: 
				return GL_FUNC_ADD;
			case blend_equation::func_subtract:
				return GL_FUNC_SUBTRACT;
			case blend_equation::func_reverse_subtract:
				return GL_FUNC_REVERSE_SUBTRACT;
			case blend_equation::min: 
				return GL_MIN;
			case blend_equation::max: 
				return GL_MAX;
			default: return 0;
		}
	}

	constexpr GLenum moka_to_gl(const buffer_usage type)
	{
		switch (type)
		{
		case buffer_usage::static_draw: return GL_STATIC_DRAW;
		case buffer_usage::dynamic_draw: return GL_DYNAMIC_DRAW;
		case buffer_usage::stream_draw: return GL_STREAM_DRAW;
		default: return 0;
		}
	}

	constexpr GLenum moka_to_gl(const blend_function_factor type)
	{
		switch (type)
		{
		case blend_function_factor::zero:
			return GL_ZERO;
		case blend_function_factor::one:
			return GL_ONE;
		case blend_function_factor::src_colour:
			return GL_SRC_COLOR;
		case blend_function_factor::one_minus_src_colour:
			return GL_ONE_MINUS_SRC_COLOR;
		case blend_function_factor::dst_colour:
			return GL_DST_COLOR;
		case blend_function_factor::one_minus_dst_colour:
			return GL_ONE_MINUS_DST_COLOR;
		case blend_function_factor::src_alpha:
			return GL_SRC_ALPHA;
		case blend_function_factor::one_minus_src_alpha:
			return GL_ONE_MINUS_SRC_ALPHA;
		case blend_function_factor::dst_alpha:
			return GL_DST_ALPHA;
		case blend_function_factor::one_minus_dst_alpha:
			return GL_ONE_MINUS_DST_ALPHA;
		case blend_function_factor::constant_colour:
			return GL_CONSTANT_COLOR;
		case blend_function_factor::one_minus_constant_colour:
			return GL_ONE_MINUS_CONSTANT_COLOR;
		case blend_function_factor::constant_alpha:
			return GL_CONSTANT_ALPHA;
		case blend_function_factor::one_minus_constant_alpha:
			return GL_ONE_MINUS_CONSTANT_ALPHA;
		default: return 0;
		}
	}

	void gl_graphics_api::visit(clear_command& cmd)
	{
		uint32_t bitmask = 0;

		if (cmd.clear_color)
		{
			auto& color = cmd.color;
			glClearColor(color.r, color.g, color.b, color.a);
			bitmask = bitmask | GL_COLOR_BUFFER_BIT;
		}

		if (cmd.clear_depth)
		{
			bitmask = bitmask | GL_DEPTH_BUFFER_BIT;
		}

		glClear(bitmask);
	}

	void gl_graphics_api::visit(viewport_command& cmd)
	{
		glViewport(cmd.x, cmd.y, cmd.width, cmd.height);
	}

	void gl_graphics_api::visit(scissor_command& cmd)
	{
		glScissor(cmd.x, cmd.y, cmd.width, cmd.height);
	}

	void gl_graphics_api::visit(fill_vertex_buffer_command& cmd)
	{
		const GLuint handle(cmd.handle.id);

		auto& data = vertex_buffer_data_[cmd.handle.id];
		data.size = cmd.size;

		glBindBuffer(GL_ARRAY_BUFFER, handle);
		glBufferData(GL_ARRAY_BUFFER
			, cmd.size
			, cmd.data
			, moka_to_gl(data.buffer_use)
		);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void gl_graphics_api::visit(fill_index_buffer_command& cmd)
	{
		const GLuint handle(cmd.handle.id);

		auto& data = index_buffer_data_[cmd.handle.id];
		data.size = cmd.size;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER
			, cmd.size
			, cmd.data
			, moka_to_gl(data.buffer_use)
		);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void gl_graphics_api::visit(draw_command& cmd)
	{
		// early exit if a bogus vertex buffer is given to us
		if(cmd.index_buffer.id == std::numeric_limits<uint16_t>::max())
		{
			return;
		}

		const auto indexed = cmd.index_buffer.id != std::numeric_limits<uint16_t>::max();

		glBindBuffer(GL_ARRAY_BUFFER, cmd.vertex_buffer.id);

		if (indexed)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cmd.index_buffer.id);
		}

		// update the current vertex layout to match the vertices we're about to render
		auto& layout = vertex_buffer_data_[cmd.vertex_buffer.id].layout;

		for (const auto& attribute : layout)
		{
			glVertexAttribPointer(
				GLuint(attribute.index),
				GLint(attribute.size),
				moka_to_gl(attribute.type),
				attribute.normalized,
				GLsizei(attribute.stride),
				reinterpret_cast<void*>(attribute.offset));

			glEnableVertexAttribArray(GLuint(attribute.index));
		}

		glEnableVertexAttribArray(0);

		auto& material = cmd.mat;
		auto& previous = previous_command_.mat;

		// if this is a different shader, update the program state
		if (material.get_program().id != previous.get_program().id)
		{
			glUseProgram(material.get_program().id);
		}

		auto& blend = material.get_blend();
		blend.enabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
		glBlendEquation(moka_to_gl(blend.equation));
		glBlendFunc(moka_to_gl(blend.source), moka_to_gl(blend.destination));

		auto& polygon_mode = material.get_polygon_mode();
		glPolygonMode(moka_to_gl(polygon_mode.faces), moka_to_gl(polygon_mode.mode));

		auto& culling = material.get_culling();
		culling.enabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
		glCullFace(moka_to_gl(culling.faces));

		material.get_depth_test() ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
		material.get_scissor_test() ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST);

		const auto size = material.size();

		size_t current_texture_unit = 0;

		for (size_t i = 0; i < size; i++)
		{
			auto& parameter = material[i];

			const auto location = glGetUniformLocation(GLuint(material.get_program().id), parameter.name.c_str());

			if (location == -1) continue;

			switch (parameter.type)
			{
			case parameter_type::texture:
			{
				const auto data = std::get<texture>(parameter.data);
				glUniform1i(location, GLint(current_texture_unit));
				glActiveTexture(GL_TEXTURE0 + GLenum(current_texture_unit));
				glBindTexture(GL_TEXTURE_2D, GLuint(data.id));
				++current_texture_unit;
				break;
			}
			case parameter_type::float32:
			{
				auto data = std::get<float>(parameter.data);
				glUniform1fv(location, GLsizei(parameter.count), &data);
				break;
			}
			case parameter_type::vec3:
			{
				auto data = std::get<glm::vec3>(parameter.data);
				glUniform3fv(location, GLsizei(parameter.count), glm::value_ptr(data));
				break;
			}
			case parameter_type::vec4:
			{
				auto data = std::get<glm::vec4>(parameter.data);
				glUniform4fv(location, GLsizei(parameter.count), glm::value_ptr(data));
				break;
			}
			case parameter_type::mat3:
			{
				auto data = std::get<glm::mat3>(parameter.data);
				glUniformMatrix3fv(location, GLsizei(parameter.count), false, glm::value_ptr(data));
				break;
			}
			case parameter_type::mat4:
			{
				auto data = std::get<glm::mat4>(parameter.data);
				glUniformMatrix4fv(location, GLsizei(parameter.count), false, glm::value_ptr(data));
				break;
			}
			default:;
			}
		}

		if (indexed)
		{
			auto& index_buffer = index_buffer_data_[cmd.index_buffer.id];

			glDrawElements(
				moka_to_gl(cmd.prim_type),
				GLsizei(cmd.index_count),
				moka_to_gl(cmd.idx_type),
				reinterpret_cast<void*>(cmd.index_buffer_offset));
		}
		else
		{
			glDrawArrays(
				moka_to_gl(cmd.prim_type),
				GLint(cmd.first_vertex),
				GLsizei(cmd.vertex_count));
		}

		previous_command_ = cmd;
	}
	
    program gl_graphics_api::make_program(const shader& vertex_handle, const shader& fragment_handle)
    {
        auto id = glCreateProgram();

		const program result{ static_cast<uint16_t>(id) };

        // Attach shaders as necessary.

        glAttachShader(id, vertex_handle.id);

        glAttachShader(id, fragment_handle.id);

        // Link the program.
        glLinkProgram(id);

        auto is_linked = 0;

        glGetProgramiv(id, GL_LINK_STATUS, &is_linked);
        if (is_linked == GL_FALSE)
        {
            auto max_length = 0;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &max_length);

            // The maxLength includes the NULL character
            char info_log[512];

            glGetProgramInfoLog(id, max_length, &max_length, &info_log[0]);

            // The program is useless now. So delete it.
            glDeleteProgram(id);

            glGetShaderInfoLog(id, 512, nullptr, info_log);

			log_.info(info_log);
        }

        return result;
    }

	shader gl_graphics_api::make_shader(const shader_type type, const std::string& source)
    {
        int success;

        char info_log[512];

		auto id = glCreateShader(moka_to_gl(type));

		const shader result{ static_cast<uint16_t>(id) };

		const char* source_chars = source.c_str();

        glShaderSource(id, 1, &source_chars, nullptr);

        glCompileShader(id);

        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            using namespace std::string_literals;

            glGetShaderInfoLog(id, 512, nullptr, info_log);
        }

        return result;
    }

    vertex_buffer gl_graphics_api::make_vertex_buffer(
		const void* vertices,
		size_t size, 
	    vertex_layout&& layout, 
		buffer_usage use)
    {
		vertex_buffer result;

		GLuint handle;

		vertex_buffer_data data;
		data.layout = std::move(layout);
		data.buffer_use = use;
		data.size = size;

		glGenBuffers(1, &handle);

		result.id = uint16_t(handle);

		vertex_buffer_data_[result.id] = std::move(data);

		glBindBuffer(GL_ARRAY_BUFFER, handle);
		glBufferData(GL_ARRAY_BUFFER
			, size
			, vertices
			, moka_to_gl(use)
		);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

        return result;
    }

	index_buffer gl_graphics_api::make_index_buffer(const void* indices, size_t size, index_type type, buffer_usage use)
	{
		index_buffer result;

		GLuint handle;

		index_buffer_data data;
		data.buffer_use = use;
		data.size = size;
		data.type = type;

		glGenBuffers(1, &handle);

		result.id = uint16_t(handle);

		index_buffer_data_[result.id] = data;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER
			, size
			, indices
			, moka_to_gl(use)
		);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return result;
	}

	void gl_graphics_api::submit(command_list&& commands)
	{
		commands.accept(*this);
	}

	void gl_graphics_api::submit_and_swap(command_list&& commands)
	{
		const auto duration = profile<milliseconds>([&]()
		{
			commands.accept(*this);
			window_.swap_buffer();
			previous_command_ = {};
		});

		int foo = 0;
	}

	// todo: add argument to make_texture to allow users to specify the internal pixel format seperately from the input format
	// todo: add arguments to allow users to specify filtering / mipmap generation
	moka::texture gl_graphics_api::make_texture(const void* pixels, const glm::ivec2& resolution, texture_components components, texture_wrap_mode wrap_mode, bool has_mipmaps)
	{
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (has_mipmaps)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resolution.x, resolution.y, 0, moka_to_gl(components), GL_UNSIGNED_BYTE, pixels);

		if (has_mipmaps)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		return moka::texture{ static_cast<uint16_t>(texture) };
	}

	void GLAPIENTRY
		gl_graphics_api::message_callback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			log_.error(message);
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			log_.warn(message);
			break;
		case GL_DEBUG_SEVERITY_LOW:
			log_.debug(message);
			break;
		default:
			log_.trace(message);
			break;
		}
	}

	logger gl_graphics_api::log_("OpenGL");

	gl_graphics_api::gl_graphics_api(window& window)
		: window_(window)
    {
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			log_.error("Failed to initialise glew: {}", glewGetErrorString(glewInit()));
			std::abort();
		}

		// enable GL_DEBUG_OUTPUT by default if it's a debug build
		if constexpr(application_traits::is_debug_build)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(message_callback, 0);
		}

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		glEnable(GL_MULTISAMPLE);
	}

	gl_graphics_api::~gl_graphics_api()
	{
		glDeleteVertexArrays(1, &vao_);
	}
}
