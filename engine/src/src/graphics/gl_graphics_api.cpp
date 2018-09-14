
#include <string>
#include <sstream>
#include <application/logger.hpp>
#include <graphics/gl_graphics_api.hpp>

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

		// if draw commands have been sent
		if (draw_call_buffer_pos_ > 0)
		{
			// sort all render items to minimise state changes
			std::sort(draw_call_buffer_.begin(), draw_call_buffer_.begin() + draw_call_buffer_pos_, comparator);

			// clear the render target
			auto color = colour::cornflower_blue();

			glClearColor(color.r(), color.g(), color.b(), color.a());
			glClear(GL_COLOR_BUFFER_BIT);

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
						const auto index = static_cast<size_t>(attribute.attr);
						const auto type = moka_to_gl(attribute.type);
						const auto size = attribute.size;
						const auto normalized = attribute.normalized;
						const auto stride = layout.stride();

						const auto offset = reinterpret_cast<void*>(
							size_offset * attr_type_size(attribute.type));

						glEnableVertexAttribArray(index);

						glVertexAttribPointer(
							index,
							size,
							type,
							normalized,
							stride,
							offset);

						size_offset += size;
					}

					glEnableVertexAttribArray(0);
				}

				if (previous_call_.program.id != current_call.program.id)
				{
					glUseProgram(current_call.program.id);
				}

				// the buffer allocates a contiguous block of space for each draw call's uniforms
				// we're interested in the block between uniform_start and uniform_end

				uniform_buffer_.set_position(current_call.uniform_start);

				// iterate over every uniform related to this draw call
				while (uniform_buffer_.position() < current_call.uniform_end)
				{
					// the handle is packed into the buffer as a header, allowing us to look up the size & type of the data that follows:

					auto handle = uniform_buffer_.read_uniform_handle(); // get the uniform handle
					auto uniform_data = uniform_data_[handle.id];		 // get the uniform type & count

																		 // now that we know the size and type of the uniform, we can access the raw data.
					const auto size = get_uniform_size(uniform_data.type, uniform_data.count);
					auto data = uniform_buffer_.read_uniform_body(size);

					//todo: abstact opengl implementation detail to gl_graphics_api

					auto location = glGetUniformLocation(GLuint(current_call.program.id), uniform_data.name.data());

					switch (uniform_data.type)
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
					glDrawElements(
						moka_to_gl(primitive_type::triangles), 
						current_call.index_count,
						GL_UNSIGNED_INT, 
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

		uniform_buffer_.clear();
		draw_call_buffer_pos_ = 0;
	}

	uniform_handle gl_graphics_api::create_uniform(const char* name, const uniform_type& type, const size_t count)
	{
		auto& uniform_data = uniform_data_[uniform_count_];
		uniform_data.type = type;
		uniform_data.name = name;
		uniform_data.count = count;
		return uniform_handle{ static_cast<handle_id>(uniform_count_++) };
	}

	const uniform_data& gl_graphics_api::set_uniform(const uniform_handle& uniform, const void* data)
	{
		auto& uniform_data = uniform_data_[uniform.id];
		uniform_data.buffer_start = uniform_buffer_.end();
		uniform_buffer_.set_uniform(uniform, uniform_data.type, data, uniform_data.count);
		uniform_data.buffer_end = uniform_buffer_.end();
		return uniform_data;
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

            log_.log(level::error, info_log);
        }

        programs_.emplace_back(result);
        return result;
    }

	shader_handle gl_graphics_api::create_shader(const shader_type type, const std::string& source)
    {
        int success;

        char info_log[512];

        // vertex shader
        auto string = source.c_str();

        const unsigned int id = glCreateShader(moka_to_gl(type));
		const shader_handle handle{ static_cast<handle_id>(id) };

        glShaderSource(id, 1, &string, nullptr);

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
                log_.log(level::error, "GLSL vertex shader compilation failed: "s + info_log);
                break;
            case shader_type::fragment:
                log_.log(level::error, "GLSL fragment shader compilation failed: "s + info_log);
                break;
            case shader_type::compute:
                log_.log(level::error, "GLSL compute shader compilation failed: "s + info_log);
                break;
            default:;
            }

        }

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

		return handle;
	}

	void GLAPIENTRY
		message_callback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
	{
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
	}

	gl_graphics_api::gl_graphics_api()
        : log_{ filesystem::current_path() }
    {
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			std::cout << glewGetErrorString(glewInit()) << std::endl;
		}

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(message_callback, 0);
	}

	gl_graphics_api::~gl_graphics_api()
	{
		glDeleteVertexArrays(1, &vao_);
	}
}
