
#include <graphics_api.hpp>
#include <logger/logger.hpp>
#include <gl_graphics_api_impl.hpp>

namespace neon
{
    std::unique_ptr<graphics_api_impl> create(
        const graphics_backend backend)
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

    graphics_api::graphics_api(
        const graphics_backend backend)
        : impl_{ create(backend) }
    {}

    vertex_buffer_handle graphics_api::create_vertex_buffer(
        const float* vertices
        , const size_t size
        , const vertex_decl& decl) const
    {
        return impl_->create_vertex_buffer(vertices, size, decl);
    }

    shader_handle graphics_api::create_shader(
        const shader_type type
        , const std::string& source) const
    {
        return impl_->create_shader(type, source);
    }

    void graphics_api::destroy(
        const shader_handle handle) const
    {
        impl_->destroy(handle);
    }

    program_handle graphics_api::create_program(
        const shader_handle vertex_handle
        , const shader_handle fragment_handle) const
    {
        return impl_->create_program(vertex_handle, fragment_handle);
    }

    program_handle graphics_api::create_program(
        shader_handle compute_handle)
    {
		return {};
    }

    void graphics_api::destroy(
        program_handle handle)
    {}

	void graphics_api::destroy(
        vertex_buffer_handle handle)
	{}

	void graphics_api::clear_colour(
        const colour& colour) const
	{
		impl_->clear_colour(colour);
	}

	void graphics_api::clear(
        const bool color
        , const bool depth
        , const bool stencil
        , const byte stencil_value
        , const colour& colour) const
	{
		impl_->clear(color, depth, stencil, stencil_value, colour);
	}

	void graphics_api::check_errors() const
	{
		impl_->check_errors();
	}

    void graphics_api::submit(const vertex_buffer_handle& vertex_buffer, const program_handle& program)
    {
        impl_->submit(vertex_buffer, program);
    }
}
