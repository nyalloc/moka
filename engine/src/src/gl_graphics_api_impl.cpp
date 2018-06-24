
#include <GL/glew.h>
#include <string>
#include <sstream>
#include <logger/logger.hpp>
#include <gl_graphics_api_impl.hpp>

namespace neon
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
            case GL_STACK_OVERFLOW:
                error = "GL_STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                error = "GL_STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                error = "GL_OUT_OF_MEMORY";
                break;
            default:
                break;
            }

        }
    }

	void gl_graphics_api_impl::clear_colour(const colour& colour) const
	{
		auto clear_flags = 0;

		glClearColor(colour.r(), colour.g(), colour.b(), colour.a());
		clear_flags |= GL_COLOR_BUFFER_BIT;

		glClear(clear_flags);
	}

    void gl_graphics_api_impl::set_scissor(const rectangle& bounds) const
    {
    }

    void gl_graphics_api_impl::set_viewport(const rectangle& bounds) const
    {
    }

    void gl_graphics_api_impl::draw_indexed_primitives(const primitive_type primitive_type, const size_t index_count) const
    {
    }

    void gl_graphics_api_impl::draw_primatives(const primitive_type primitive_type, const size_t vertex_count) const
    {
    }

    void gl_graphics_api_impl::scissor_test(const toggle toggle) const
    {
    }

    void gl_graphics_api_impl::depth_mask(const toggle toggle) const
    {
    }

    void gl_graphics_api_impl::depth_test(const toggle toggle) const
    {
    }

    void gl_graphics_api_impl::face_culling(const toggle toggle) const
    {
    }

    void gl_graphics_api_impl::blend(const toggle toggle) const
    {
    }

    void gl_graphics_api_impl::blend_function(const blend_function_factor lhs, const blend_function_factor rhs) const
    {
    }

    program_handle gl_graphics_api_impl::create_program(const shader_handle& vertex_handle, const shader_handle& fragment_handle)
    {
        const program_handle result{ programs_.size() };

        const auto program = glCreateProgram();

        // Attach shaders as necessary.
        glAttachShader(program, shaders_[vertex_handle.id]);

        glAttachShader(program, shaders_[fragment_handle.id]);

        // Link the program.
        glLinkProgram(program);

        auto isLinked = 0;

        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            auto maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            // The program is useless now. So delete it.
            glDeleteProgram(program);

            // Provide the infolog in whatever manner you deem best.
            // Exit with failure.
        }

        programs_[result.id] = program;

        return result;
    }

    void gl_graphics_api_impl::destroy(const shader_handle& handle)
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

    void gl_graphics_api_impl::clear(const bool color, const bool depth, const bool stencil, const byte stencil_value, const colour& colour) const
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

    void gl_graphics_api_impl::viewport(const rectangle& rectangle) const
    {
        glViewport(rectangle.x, rectangle.y, rectangle.width, rectangle.height);
    }

    void gl_graphics_api_impl::scissor(const rectangle& rectangle) const
    {
        glScissor(rectangle.x, rectangle.y, rectangle.width, rectangle.height);
    }
}
