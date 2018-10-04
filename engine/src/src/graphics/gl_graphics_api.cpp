
#include <string>
#include <sstream>
#include <application/logger.hpp>
#include <graphics/gl_graphics_api.hpp>
#include <filesystem>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <graphics/material.hpp>

namespace moka
{
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
	};

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
	};


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
	};

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
	};

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
	};

	void gl_graphics_api::submit(draw_call&& call) 
	{
		// lock here!
		if (draw_call_buffer_pos_ + 1 < draw_call_buffer_.size())
		{
			draw_call_buffer_[draw_call_buffer_pos_++] = std::move(call);
		}
	}

	void gl_graphics_api::frame()
	{
		// draw item comparator for sorting
		auto comparator = [](draw_call& a, draw_call& b)
		{
			return a.key > b.key;
		};

		// clear the render target
		auto color = colour::light_grey();

		glClearColor(color.r(), color.g(), color.b(), color.a());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// if draw commands have been sent
		if (draw_call_buffer_pos_ > 0)
		{
			// sort all render items to minimise state changes
			std::sort(draw_call_buffer_.begin(), draw_call_buffer_.begin() + draw_call_buffer_pos_, comparator);

			for (auto it = draw_call_buffer_.begin(); it != draw_call_buffer_.begin() + draw_call_buffer_pos_; ++it)
			{
				auto& current_call = *it;

				if (previous_call_.vertex_buffer.id != current_call.vertex_buffer.id)
				{
					if (!is_handle_valid(current_call.vertex_buffer))
					{
						continue;
					}

					glBindBuffer(GL_ARRAY_BUFFER, current_call.vertex_buffer.id);

					if (is_handle_valid(current_call.index_buffer))
					{
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, current_call.index_buffer.id);
					}

					// update the current vertex layout to match the vertices we're about to render
					size_t size_offset = 0;
					auto layout = vertex_layouts_[current_call.vertex_buffer.id];

					for (const attribute_element& attribute : layout)
					{
						glVertexAttribPointer(
							attribute.index,
							attribute.size,
							moka_to_gl(attribute.type),
							attribute.normalized,
							attribute.stride,
							reinterpret_cast<void*>(attribute.offset));

						glEnableVertexAttribArray(attribute.index);
					}

					glEnableVertexAttribArray(0);
				}

				auto effect_ptr = current_call.material;

				if (effect_ptr)
				{
					auto effect = *effect_ptr;

					// if this is the first frame (previous call effect will be nullptr)
					// or the previous effect is different from the current one, update the current program state
					if (!previous_call_.material || previous_call_.material->get_program().id != effect.get_program().id)
					{
						glUseProgram(effect.get_program().id);
					}

					auto program = effect_ptr->get_program();
					auto size = effect_ptr->size();

					size_t current_texture_unit = 0;

					for (auto i = 0; i < size; i++)
					{
						auto& parameter = effect[i];

						auto location = glGetUniformLocation(GLuint(effect.get_program().id), parameter.name.c_str());

						if (location == -1) continue;

						switch (parameter.type)
						{
						case parameter_type::texture:
						{
							auto data = std::get<texture_2d>(parameter.data);
							// FIX THIS!!! TEXTURE UNIT 0???
							glUniform1i(location, current_texture_unit);
							glActiveTexture(GL_TEXTURE0 + current_texture_unit);
							glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(data.handle.id));
							++current_texture_unit;
							break;
						}
						case parameter_type::float32:
						{
							auto data = std::get<float>(parameter.data);
							glUniform1fv(location, parameter.count, &data);
							break;
						}
						case parameter_type::vec3:
						{
							auto data = std::get<glm::vec3>(parameter.data);
							glUniform3fv(location, parameter.count, glm::value_ptr(data));
							break;
						}
						case parameter_type::vec4:
						{
							auto data = std::get<glm::vec4>(parameter.data);
							glUniform4fv(location, parameter.count, glm::value_ptr(data));
							break;
						}
						case parameter_type::mat3:
						{
							auto data = std::get<glm::mat3>(parameter.data);
							glUniformMatrix3fv(location, parameter.count, false, glm::value_ptr(data));
							break;
						}
						case parameter_type::mat4:
						{
							auto data = std::get<glm::mat4>(parameter.data);
							glUniformMatrix4fv(location, parameter.count, false, glm::value_ptr(data));
							break;
						}
						default:;
						}
					}

					current_texture_unit = 0;
				}

				// if an index buffer is set, draw indexed. Otherwise, draw arrays.
				if (is_handle_valid(current_call.index_buffer))
				{
					glDrawElements(
						moka_to_gl(primitive_type::triangles), 
						current_call.index_count,
						GL_UNSIGNED_SHORT, 
						nullptr);
				}
				else
				{
					glDrawArrays(
						moka_to_gl(primitive_type::triangles), 
						current_call.vertex_start, 
						current_call.vertex_count);
				}

				previous_call_ = current_call;
			}
		}

		draw_call_buffer_pos_ = 0;
	}

    program_handle gl_graphics_api::create_program(const shader_handle& vertex_handle, const shader_handle& fragment_handle)
    {
        const auto id = glCreateProgram();
		const program_handle result{ static_cast<handle_id>(id) };

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

		log_.info("OpenGL program sucessfully created. Handle: {}", result.id);

        programs_.emplace_back(result);
        return result;
    }

	shader_handle gl_graphics_api::create_shader(const shader_type type, const std::string& source)
    {
        int success;

        char info_log[512];

        // vertex shader

        const unsigned int id = glCreateShader(moka_to_gl(type));
		const shader_handle handle{ static_cast<handle_id>(id) };

		const char* source_chars = source.c_str();

        glShaderSource(id, 1, &source_chars, nullptr);

        glCompileShader(id);

        // print compile errors if any
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            using namespace std::string_literals;

            glGetShaderInfoLog(id, 512, nullptr, info_log);

            switch (type)
            {
            case shader_type::vertex:
				log_.error("GLSL vertex shader compilation failed: {}",  info_log);
                break;
            case shader_type::fragment:
				log_.error("GLSL fragment shader compilation failed: {}", info_log);
				break;
            case shader_type::compute:
				log_.error("GLSL fragment shader compilation failed: {}", info_log);
				break;
            default:;
            }

        }

		log_.info("OpenGL shader sucessfully created. Handle: {}", handle.id);

        shaders_.emplace_back(handle);
        return handle;
    }

    vertex_buffer_handle gl_graphics_api::create_vertex_buffer(
		const void* vertices,
		size_t size, 
		const vertex_layout& layout)
    {
		GLuint id;
		glGenBuffers(1, &id);

		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER
			, size
			, vertices
			, (NULL == vertices) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW
		);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		const vertex_buffer_handle handle{ static_cast<handle_id>(id) };

		vertex_layouts_[handle.id] = layout;
        vertex_buffers_.emplace_back(handle);

		log_.info("OpenGL vertex buffer sucessfully created. Handle: {}", handle.id);

        return handle;
    }

	index_buffer_handle gl_graphics_api::create_index_buffer(const void* indices, size_t size)
	{
		GLuint id;
		glGenBuffers(1, &id);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER
			, size
			, indices
			, (NULL == indices) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW
		);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		const index_buffer_handle handle{ static_cast<handle_id>(id) };

		index_buffers_.emplace_back(handle);

		log_.info("OpenGL index buffer sucessfully created. Handle: {}", handle.id);

		return handle;
	}

	// todo: add argument to create_texture to allow users to specify the internal pixel format seperately from the input format
	// todo: add arguments to allow users to specify filtering / mipmap generation
	texture_handle gl_graphics_api::create_texture(const texture_data & data)
	{
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.resolution.x(), data.resolution.y(), 0, moka_to_gl(data.components), GL_UNSIGNED_BYTE, data.data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return texture_handle{ static_cast<handle_id>(texture) };
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
		log_.error(message);
	}

	logger gl_graphics_api::log_("OpenGL");

	gl_graphics_api::gl_graphics_api()
    {
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			log_.error("Failed to initialise glew: {}", glewGetErrorString(glewInit()));
		}

		// enable GL_DEBUG_OUTPUT by default if it's a debug build
		if constexpr(application_traits::is_debug_build)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(message_callback, 0);
		}

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	gl_graphics_api::~gl_graphics_api()
	{
		glDeleteVertexArrays(1, &vao_);
	}
}
