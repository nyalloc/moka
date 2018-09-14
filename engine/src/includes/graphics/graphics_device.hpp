#pragma once

#include <memory>
#include <graphics/graphics_api.hpp>
#include <vector>
#include "messaging/receiver.hpp"
#include "application/window.hpp"
#include "draw_call_builder.hpp"

namespace moka
{
    /**
     * \brief Specifies what native rendering API to use as the \p moka::graphics_api backend.
     */
    enum class graphics_backend
    {
        direct3d_9,  //!< Direct3D 9.0
        direct3d_11, //!< Direct3D 11.0
        direct3d_12, //!< Direct3D 12.0
        gnm,         //!< GNM
        metal,       //!< Metal
        opengl_es,   //!< OpenGL ES 2.0+
        opengl,      //!< OpenGL 2.1+
        vulkan,      //!< Vulkan
        null         //!< No rendering API specified
    };

    /**
     * \brief The \p moka::graphics_device class presents a common rendering API and owns a pointer to the implementation.
     * An example of the bridge design pattern, \p moka::graphics_api allows moka rendering code to operate without being coupled to the native rendering API.
     * The client interacts with this class and it in turn delegates all requests to the \p moka::graphics_api class internally.
     * \p graphics_api is the handle known and used by the client, while \p moka::graphics_api_impl is safely encapsulated to ensure that it may continue to evolve, or be entirely replaced.
     */
    class graphics_device : public receiver
    {
		window& window_;					//<! window that owns the rendering context
		context_handle worker_context_;		//<! rendering context of the renderer worker thread
		context_handle main_context_;		//<! rendering context of the main thread
		graphics_backend graphics_backend_; //<! renderer backend type enum

		std::thread worker_;					     //<! rendering thread (only thread to interact with the backend API)
		std::unique_ptr<graphics_api> graphics_api_; //<! polymorphic abstraction of the native rendering API

    	void worker_thread();
    public:
        graphics_device(
			window& window,
			const graphics_backend graphics_backend = graphics_backend::opengl);

        vertex_buffer_handle create_vertex_buffer(
			const void* vertices, 
			const size_t size,
			const vertex_layout& layout);

		index_buffer_handle create_index_buffer(
			const void* indices, 
			size_t size);

		void submit(draw_call&& call);

        void destroy(shader_handle handle) const;

		shader_handle create_shader(const shader_type type, const std::string& source);

		draw_call_builder draw();

        /**
         * \brief Create program with vertex and fragment shaders.
         * \param vertex_handle Vertex shader.
         * \param fragment_handle Fragment shader.
         * \return Program handle if vertex shader output and fragment shader input are matching, otherwise returns invalid program handle.
         */
        program_handle create_program(shader_handle vertex_handle, shader_handle fragment_handle);

        /**
         * \brief Create program with compute shader.
         * \param compute_handle Compute shader.
         * \return Program handle.
         */
        program_handle create_program(shader_handle compute_handle);

        /**
         * \brief Destroy program.
         * \param handle Program handle.
         */
        void destroy(program_handle handle);

        /**
         * \brief Destroy vertex buffer.
         * \param handle Vertex buffer handle.
         */
        void destroy(vertex_buffer_handle handle);

		/**
	     * \brief Destroy index buffer.
		 * \param handle Index buffer handle.
		 */
		void destroy(index_buffer_handle handle);

		void frame();

		uniform_handle create_uniform(const char* name, const uniform_type& type, size_t size = 1);

	    const uniform_data& set_uniform(const uniform_handle& uniform, const void* data);
    };
}
