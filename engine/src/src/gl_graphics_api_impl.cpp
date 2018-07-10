
#include <GL/glew.h>
#include <string>
#include <sstream>
#include <logger/logger.hpp>
#include <gl_graphics_api_impl.hpp>

namespace moka
{
    void gl_graphics_api_impl::check_errors() const
    {
        std::string error;

        // check for up to 10 errors pending
        for (auto i = 0; i < 10; i++)
        {
            const int err = glGetError();
            if (err == GL_NO_ERROR)
            {
                return;
            }
            switch (err)
            {
            case GL_INVALID_ENUM:
                error = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "GL_INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                error = "GL_OUT_OF_MEMORY";
                break;
            case GL_STACK_OVERFLOW:
                error = "GL_STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                error = "GL_STACK_UNDERFLOW";
                break;
            default:
                break;
            }

            log_.log(level::error, "OpenGL error: " + error);
        }
    }

    void gl_graphics_api_impl::clear_colour(const colour& colour) const
    {
        glClearColor(colour.r(), colour.g(), colour.b(), colour.a());
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void gl_graphics_api_impl::set_scissor(const rectangle& bounds) const
    {
        glScissor(bounds.x(), bounds.y(), bounds.width(), bounds.height());
    }

    void gl_graphics_api_impl::set_viewport(const rectangle& bounds) const
    {
        glViewport(bounds.x(), bounds.y(), bounds.width(), bounds.height());
    }

    void gl_graphics_api_impl::draw_indexed_primitives(const primitive_type primitive_type, const size_t index_count) const
    {
    }

    void gl_graphics_api_impl::draw_primatives(const primitive_type primitive_type, const size_t vertex_count) const
    {
    }

    void gl_graphics_api_impl::scissor_test(const toggle toggle) const
    {
        switch (toggle)
        {
        case toggle::enable:
            glEnable(GL_SCISSOR_TEST);
            break;
        case toggle::disable:
            glDisable(GL_SCISSOR_TEST);
            break;
        default:;
        }
    }

    void gl_graphics_api_impl::depth_mask(const toggle toggle) const
    {
        switch (toggle)
        {
        case toggle::enable:
            glDepthMask(GL_TRUE);
            break;
        case toggle::disable:
            glDepthMask(GL_FALSE);
            break;
        default:;
        }
    }

    void gl_graphics_api_impl::depth_test(const toggle toggle) const
    {
        switch (toggle)
        {
        case toggle::enable:
            glEnable(GL_DEPTH_TEST);
            break;
        case toggle::disable:
            glDisable(GL_DEPTH_TEST);
            break;
        default:;
        }
    }

    void gl_graphics_api_impl::face_culling(const toggle toggle) const
    {
        switch (toggle)
        {
        case toggle::enable:
            glEnable(GL_CULL_FACE);
            break;
        case toggle::disable:
            glDisable(GL_CULL_FACE);
            break;
        default:;
        }
    }

    void gl_graphics_api_impl::blend(const toggle toggle) const
    {
        switch (toggle)
        {
        case toggle::enable:
            glEnable(GL_BLEND);
            break;
        case toggle::disable:
            glDisable(GL_BLEND);
            break;
        default:;
        }
    }

    void gl_graphics_api_impl::blend_function(const blend_function_factor lhs, const blend_function_factor rhs) const
    {
        auto moka_to_gl = [](const blend_function_factor type)
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
            default:;
            }
        };

        glBlendFunc(moka_to_gl(lhs), moka_to_gl(rhs));
    }

    program_handle gl_graphics_api_impl::create_program(const shader_handle& vertex_handle, const shader_handle& fragment_handle)
    {
        const auto id = glCreateProgram();
        const program_handle result{ id };

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

    void gl_graphics_api_impl::destroy(const shader_handle& handle)
    {
    }

    shader_handle gl_graphics_api_impl::create_shader(const shader_type type, const std::string& source)
    {
        auto moka_to_gl = [](const shader_type type)
        {
            switch (type)
            {
            case shader_type::vertex:
                return GL_VERTEX_SHADER;
            case shader_type::fragment:
                return GL_FRAGMENT_SHADER;
            case shader_type::compute:
                return GL_COMPUTE_SHADER;
            default:;
            }
        };

        int success;

        char info_log[512];

        // vertex shader
        auto string = source.c_str();

        const unsigned int id = glCreateShader(moka_to_gl(type));
        const shader_handle handle{ id };

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

    vertex_buffer_handle gl_graphics_api_impl::create_vertex_buffer(
        const memory& vertices,
        const vertex_decl& decl)
    {
        auto moka_to_gl = [](const attribute_type type)
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
            default:;
            }
        };

        unsigned int vbo, vao;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, vertices.size, vertices.data, GL_STATIC_DRAW);

        size_t size_offset = 0;
        for (const attribute_element& attribute : decl)
        {
            const auto index = static_cast<size_t>(attribute.attr);
            const auto type = moka_to_gl(attribute.type);
            const auto size = attribute.size;
            const auto normalized = attribute.normalized;
            const auto stride = decl.stride();

            const auto offset = reinterpret_cast<void*>(
                size_offset * attr_type_size(attribute.type));

            glVertexAttribPointer(
                index,
                size,
                type,
                normalized,
                stride,
                offset);

            glEnableVertexAttribArray(index);

            size_offset += size;

            // todo: investigate cleaning up attribute_element void* memory
        }

        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);

        const vertex_buffer_handle handle{ vao };

        vertex_buffers_.emplace_back(handle);
        return handle;
    }

    void gl_graphics_api_impl::submit(const vertex_buffer_handle& vertex_buffer, const program_handle& program)
    {
        glUseProgram(program.id);
        glBindVertexArray(vertex_buffer.id);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    gl_graphics_api_impl::gl_graphics_api_impl()
        : log_{ filesystem::current_path() }
    {
    }

    std::string gl_graphics_api_impl::version() const
    {
        return reinterpret_cast<const char*>(glGetString(GL_VERSION));
    }

    std::string gl_graphics_api_impl::vendor() const
    {
        return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    }

    std::string gl_graphics_api_impl::renderer() const
    {
        return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    }

    std::string gl_graphics_api_impl::extensions() const
    {
        auto count = 0;

        glGetIntegerv(GL_NUM_EXTENSIONS, &count);

        std::stringstream string;

        for (size_t i = 0; i < count; i++)
        {
            string << glGetStringi(GL_EXTENSIONS, i) << std::endl;
        }

        return string.str();
    }

    std::string gl_graphics_api_impl::shading_language_version() const
    {
        return reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
    }

    void gl_graphics_api_impl::clear(const bool color, const bool depth, const bool stencil,
        const byte stencil_value, const colour& colour) const
    {
        auto clear_flags = 0;
        if (color)
        {
            glClearColor(colour.r(), colour.g(), colour.b(), colour.a());
            clear_flags |= GL_COLOR_BUFFER_BIT;
        }
        if (depth)
        {
            clear_flags |= GL_DEPTH_BUFFER_BIT;
        }
        if (stencil)
        {
            glClearStencil(stencil_value);
            clear_flags |= GL_STENCIL_BUFFER_BIT;
        }
        glClear(clear_flags);
    }

    void gl_graphics_api_impl::depth_bounds_test(const float zmin, const float zmax) const
    {
        if (zmin > zmax)
        {
            return;
        }

        if (zmin == 0.0f && zmax == 0.0f)
        {
            glDisable(GL_DEPTH_BOUNDS_TEST_EXT);
        }
        else
        {
            glEnable(GL_DEPTH_BOUNDS_TEST_EXT);
            glDepthBoundsEXT(zmin, zmax);
        }
    }

    void gl_graphics_api_impl::polygon_offset(const float scale, const float bias) const
    {
        glPolygonOffset(scale, bias);
    }
}
