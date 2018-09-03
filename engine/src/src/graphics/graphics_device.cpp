
#include <graphics/graphics_device.hpp>
#include <logger/logger.hpp>
#include <graphics/gl_graphics_api.hpp>

namespace moka
{
    std::unique_ptr<graphics_api> create(
        const graphics_backend backend)
    {
        switch (backend)
        {
        case graphics_backend::direct3d_9:  return std::make_unique<gl_graphics_api>();
        case graphics_backend::direct3d_11: return std::make_unique<gl_graphics_api>();
        case graphics_backend::direct3d_12: return std::make_unique<gl_graphics_api>();
        case graphics_backend::gnm:         return std::make_unique<gl_graphics_api>();
        case graphics_backend::metal:       return std::make_unique<gl_graphics_api>();
        case graphics_backend::opengl_es:   return std::make_unique<gl_graphics_api>();
        case graphics_backend::opengl:      return std::make_unique<gl_graphics_api>();
        case graphics_backend::vulkan:      return std::make_unique<gl_graphics_api>();
        case graphics_backend::null:        return std::make_unique<gl_graphics_api>();
        default:                            return std::make_unique<gl_graphics_api>();
        }
    }

    graphics_device::graphics_device(
        const graphics_backend backend)
        : impl_{ create(backend) }
    {
		//// steal context from main thread
		//SDL_GL_MakeCurrent(window, sdl_gl_context);

		//auto bg = moka::color::cornflower_blue();

		//// draw item comparator for sorting
		//auto comparator = [](moka::draw_call& a, moka::draw_call& b)
		//{
		//	return a.key > b.key;
		//};

		//while (true)
		//{
		//	// execute commands for creating graphics resources
		//	// these can be returned to the caller via callback std::function
		//	graphics_receiver.poll()
		//	.handle<moka::create_program>([&](const moka::create_program& e)
		//	{
		//		// create shader program from args, return it via callback
		//		moka::program_handle h;
		//		e(h);
		//	});

		//	if (moka::buffer_n > 0)
		//	{
		//		// sort all render items to minimise state changes
		//		// http://realtimecollisiondetection.net/blog/?p=86
		//		// only iterate over items actually submitted for rendering (begin to begin + n)
		//		std::sort(moka::buffer.begin(), moka::buffer.begin() + moka::buffer_n, comparator);

		//		// clear the render target 
		//		// (investigate how these non-draw commands are submitted and sorted)
		//		glClearColor(bg.r(), bg.g(), bg.b(), bg.a());
		//		glClear(GL_COLOR_BUFFER_BIT);

		//		for (auto it = moka::buffer.begin(); it != moka::buffer.begin() + moka::buffer_n; ++it)
		//		{
		//			const auto& item = *it;

		//			graphics.draw_indexed(moka::primitive_type::triangles, 0, 3);
		//		}

		//		moka::buffer_n = 0;
		//	}

		//	// swap the buffer
		//	SDL_GL_SwapWindow(window);
		//}
    }

    vertex_buffer_handle graphics_device::create_vertex_buffer(
        const memory& vertices
        , const vertex_decl& decl) const
    {
        return impl_->create_vertex_buffer(vertices, decl);
    }

    shader_handle graphics_device::create_shader(
        const shader_type type
        , const std::string& source) const
    {
        return impl_->create_shader(type, source);
    }

    void graphics_device::destroy(
        const shader_handle handle) const
    {
        impl_->destroy(handle);
    }

    program_handle graphics_device::create_program(
        const shader_handle vertex_handle
        , const shader_handle fragment_handle) const
    {
        return impl_->create_program(vertex_handle, fragment_handle);
    }

    program_handle graphics_device::create_program(
        shader_handle compute_handle)
    {
		return {};
    }

    void graphics_device::destroy(
        program_handle handle)
    {}

	void graphics_device::destroy(
        vertex_buffer_handle handle)
	{}

	void graphics_device::clear_colour(
        const colour& colour) const
	{
		impl_->clear_colour(colour);
	}

	void graphics_device::clear(
        const bool color
        , const bool depth
        , const bool stencil
        , const byte stencil_value
        , const colour& colour) const
	{
		impl_->clear(color, depth, stencil, stencil_value, colour);
	}

	void graphics_device::check_errors() const
	{
		impl_->check_errors();
	}

	void graphics_device::bind(const vertex_buffer_handle& vertex_buffer) const
    {
		impl_->bind(vertex_buffer);
    }

	void graphics_device::bind(const texture_handle& texture) const
	{
		//impl_->bind(program);
	}

	void graphics_device::bind(const program_handle& program) const
	{
		impl_->bind(program);
	}

	void graphics_device::draw_indexed(const primitive_type type, size_t first, size_t count) const
    {
		impl_->draw_indexed(type, first, count);
    }

	void graphics_device::set_face_culling(face_culling face_culling) const
	{
		//impl_->face_culling(face_culling);
	}
}
