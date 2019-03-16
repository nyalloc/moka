#pragma once

#include <GL/glew.h>
#include <application/logger.hpp>
#include <application/window.hpp>
#include <graphics/api/graphics_api.hpp>
#include <graphics/buffer/buffer_usage.hpp>
#include <graphics/buffer/index_buffer_handle.hpp>
#include <graphics/buffer/vertex_buffer_handle.hpp>
#include <graphics/buffer/vertex_layout.hpp>
#include <graphics/command/draw_command.hpp>
#include <graphics/device/graphics_device.hpp>
#include <graphics/material/material.hpp>
#include <graphics/program.hpp>
#include <graphics/shader.hpp>

namespace moka
{
    /**
     * \brief Contains data that describes the size, use and layout of a vertex buffer
     */
    struct vertex_metadata final
    {
        buffer_usage buffer_use = buffer_usage::static_draw;
        size_t size{};
        vertex_layout layout;
    };

    /**
     * \brief Contains data that describes the size, use and layout of an index buffer
     */
    struct index_metadata final
    {
        buffer_usage buffer_use = buffer_usage::static_draw;
        size_t size{};
        index_type type = index_type::uint16;
    };

    /**
     * \brief Graphics device forward declaration
     */
    class graphics_device;

    /**
     * \brief Convert the interface of OpenGL into the moka rendering API.
     * Allows moka rendering functionality to work at a higher level without being coupled to an OpenGL backend.
     */
    class gl_graphics_api final : public graphics_api
    {
        window& window_;
        graphics_device& device_;

        static logger log_;

        GLuint vao_ = 0;

        static void GLAPIENTRY message_callback(
            GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param);

        std::unordered_map<uint16_t, vertex_metadata> vertex_buffer_data_;
        std::unordered_map<uint16_t, texture_metadata> texture_data_;
        std::unordered_map<uint16_t, index_metadata> index_buffer_data_;

        draw_command previous_command_;

        static void reset_gl_state();

        void visit(clear_command& cmd) override;

        void visit(draw_command& cmd) override;

        void visit(viewport_command& cmd) override;

        void visit(scissor_command& cmd) override;

        void visit(fill_vertex_buffer_command& cmd) override;

        void visit(fill_index_buffer_command& cmd) override;

        void visit(frame_buffer_command& cmd) override;

        void visit(frame_buffer_texture_command& cmd) override;

        void visit(generate_mipmaps_command& cmd) override;

        void visit(set_material_parameters_command& cmd) override;

        static void check_errors(const char* caller);

    public:
        /**
         * \brief Create a new gl_graphics_api object
         * \param window The window object that this gl_graphics_api will render to
         * \param device The graphics_device object that owns this api object
         */
        gl_graphics_api(window& window, graphics_device& device);

        gl_graphics_api(const gl_graphics_api& gl_graphics_api) = delete;
        gl_graphics_api(gl_graphics_api&& gl_graphics_api) = delete;
        gl_graphics_api& operator=(const gl_graphics_api& gl_graphics_api) = delete;
        gl_graphics_api& operator=(gl_graphics_api&& gl_graphics_api) = delete;

        virtual ~gl_graphics_api();

        /**
         * \brief Create a shader program from vertex & fragment shaders.
         * \param vertex_handle The vertex shader that you want to link to this program.
         * \param fragment_handle The fragment shader that you want to link to this program.
         * \return A new program_handle representing a program on the device.
         */
        program_handle make_program(const shader_handle& vertex_handle, const shader_handle& fragment_handle) override;

        /**
         * \brief Create a shader from source code.
         * \param type The type of shader you want to create.
         * \param source The source code of the shader you want to create.
         * \return A new shader_handle representing a shader on the device.
         */
        shader_handle make_shader(shader_type type, const std::string& source) override;

        /**
         * \brief Create a new vertex buffer.
         * \param vertices The host memory buffer that will be used as vertex data.
         * \param size The size of the host vertex buffer.
         * \param layout The layout of the vertex data.
         * \param use A buffer usage hint.
         * \return A new vertex_buffer_handle representing an vertex buffer on the device.
         */
        vertex_buffer_handle make_vertex_buffer(const void* vertices, size_t size, vertex_layout&& layout, buffer_usage use) override;

        /**
         * \brief Create a new index buffer.
         * \param indices The host memory buffer that will be used as index data.
         * \param size The size of the host index buffer.
         * \param type The layout of the index data.
         * \param use A buffer usage hint.
         * \return A new index_buffer_handle representing an index buffer on the device.
         */
        index_buffer_handle make_index_buffer(const void* indices, size_t size, index_type type, buffer_usage use) override;

        /**
         * \brief Create a new texture.
         * \param data The host memory buffer that will be used as texture data.
         * \param metadata Metadata describing the texture data.
         * \param free_host_data If true, free the host memory after uploading to the device. Otherwise allow the calling code to free it.
         * \return A new texture_handle representing a texture on the device.
         */
        texture_handle make_texture(void** data, texture_metadata&& metadata, bool free_host_data) override;

        /**
         * \brief Create a new frame buffer.
         * \param render_textures An array of render_texture_data.
         * \param render_texture_count Size of the render_textures array.
         * \return A new frame_buffer_handle representing a frame buffer on the device.
         */
        frame_buffer_handle make_frame_buffer(render_texture_data* render_textures, size_t render_texture_count) override;

        /**
         * \brief Submit a command_list to execute on the device.
         * \param commands The command_list you wish to run.
         */
        void submit(command_list&& commands) override;

        /**
         * \brief Submit a command_list to execute on the device. The main framebuffer will be swapped after executing, advancing a frame.
         * \param commands The command_list you wish to run.
         */
        void submit_and_swap(command_list&& commands) override;
    };
} // namespace moka
