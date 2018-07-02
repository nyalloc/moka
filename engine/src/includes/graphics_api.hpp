#pragma once

#include <memory>
#include <graphics_api_impl.hpp>
#include <vector>

namespace neon
{
    using memory = std::vector<std::byte>;

    /**
     * \brief Specifies what native rendering API to use as the \p neon::graphics_api backend.
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
     * \brief The \p neon::graphics_api class presents a common rendering API and owns a pointer to the implementation.
     * An example of the bridge design pattern, \p neon::graphics_api allows neon rendering code to operate without being coupled to the native rendering API.
     * The client interacts with this class and it in turn delegates all requests to the \p neon::graphics_api_impl class internally.
     * \p graphics_api is the handle known and used by the client, while \p neon::graphics_api_impl is safely encapsulated to ensure that it may continue to evolve, or be entirely replaced.
     */
    class graphics_api
    {
        std::unique_ptr<graphics_api_impl> impl_;
    public:
        graphics_api(graphics_backend backend = graphics_backend::opengl);

        vertex_buffer_handle create_vertex_buffer(const float* vertices, size_t sizev, const vertex_decl& decl) const;

        shader_handle create_shader(const shader_type type, const std::string& source) const;

        void destroy(shader_handle handle) const;

        /**
         * \brief Create program with vertex and fragment shaders.
         * \param vertex_handle Vertex shader.
         * \param fragment_handle Fragment shader.
         * \return Program handle if vertex shader output and fragment shader input are matching, otherwise returns invalid program handle.
         */
        program_handle create_program(shader_handle vertex_handle, shader_handle fragment_handle) const;

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

        void clear_colour(const colour& colour) const;

        void clear(bool color, bool depth, bool stencil, byte stencil_value, const colour& colour) const;

        void check_errors() const;

        void submit(const vertex_buffer_handle& vertex_buffer, const program_handle& program);
    };
}
