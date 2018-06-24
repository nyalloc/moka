
#include <graphics_api.hpp>
#include <logger/logger.hpp>
#include <gl_graphics_api_impl.hpp>

namespace neon
{
    std::unique_ptr<graphics_api_impl> create(const graphics_backend backend)
    {
        switch (backend)
        {
        case graphics_backend::direct3d_9: return nullptr;
        case graphics_backend::direct3d_11: return nullptr;
        case graphics_backend::direct3d_12: return nullptr;
        case graphics_backend::gnm: return nullptr;
        case graphics_backend::metal: return nullptr;
        case graphics_backend::opengl_es: return nullptr;
        case graphics_backend::opengl: return std::make_unique<gl_graphics_api_impl>();
        case graphics_backend::vulkan: return nullptr;
        case graphics_backend::null: return nullptr;
        default: return nullptr;
        }
    }

    graphics_api::graphics_api(const graphics_backend backend)
        : implementor_{ create(backend) }
    {}

    vertex_buffer_handle graphics_api::create_vertex()
    {
        return {};
    }

    shader_handle graphics_api::create_shader(const std::string& source)
    {
        return {};
    }

    void graphics_api::destroy(const shader_handle handle) const
    {
        implementor_->destroy(handle);
    }

    program_handle graphics_api::create_program(shader_handle vertex_handle, shader_handle fragment_handle) const
    {
        return implementor_->create_program(vertex_handle, fragment_handle);
    }

    program_handle graphics_api::create_program(shader_handle compute_handle)
    {
		return {};
    }

    void graphics_api::destroy(program_handle handle)
    {}

	void graphics_api::destroy(vertex_buffer_handle handle)
	{}

	void graphics_api::clear_colour(const colour& colour) const
	{
		implementor_->clear_colour(colour);
	}

	void graphics_api::clear(const bool color, const bool depth, const bool stencil, const byte stencil_value, const colour& colour) const
	{
		implementor_->clear(color, depth, stencil, stencil_value, colour);
	}
}
