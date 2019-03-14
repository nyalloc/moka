#pragma once

#include <GL/glew.h>
#include <application/logger.hpp>
#include <application/window.hpp>
#include <graphics/api/graphics_api.hpp>
#include <graphics/buffer/buffer_usage.hpp>
#include <graphics/buffer/index_buffer.hpp>
#include <graphics/buffer/vertex_buffer.hpp>
#include <graphics/buffer/vertex_layout.hpp>
#include <graphics/command/draw_command.hpp>
#include <graphics/device/graphics_device.hpp>
#include <graphics/material/material.hpp>
#include <graphics/program.hpp>
#include <graphics/shader.hpp>

namespace moka
{
    struct program_data final
    {
    };

    struct shader_data final
    {
        shader_type type;
    };

    struct vertex_metadata final
    {
        buffer_usage buffer_use = buffer_usage::static_draw;
        size_t size{};
        vertex_layout layout;
    };

    struct index_metadata final
    {
        buffer_usage buffer_use = buffer_usage::static_draw;
        size_t size{};
        index_type type = index_type::uint16;
    };

    class graphics_device;

    /**
     * \brief Convert the interface of OpenGL into the moka rendring API.
     * Allows moka rendering functionality to work at a higher level without being coupled to an OpenGL backend.
     */
    class gl_graphics_api final : public graphics_api
    {
        window& window_;
        graphics_device& device_;

        static logger log_;

        GLuint vao_ = 0;

        static void GLAPIENTRY message_callback(
            GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar* message,
            const void* user_param);

        std::unordered_map<uint16_t, vertex_metadata> vertex_buffer_data_;
        std::unordered_map<uint16_t, texture_metadata> texture_data_;
        std::unordered_map<uint16_t, index_metadata> index_buffer_data_;

        draw_command previous_command_;

    public:
        gl_graphics_api(window& window, graphics_device& device_);

        gl_graphics_api(const gl_graphics_api& gl_graphics_api) = delete;
        gl_graphics_api(gl_graphics_api&& gl_graphics_api) = delete;
        gl_graphics_api& operator=(const gl_graphics_api& gl_graphics_api) = delete;
        gl_graphics_api& operator=(gl_graphics_api&& gl_graphics_api) = delete;

        virtual ~gl_graphics_api();

        program_handle make_program(const shader_handle& vertex_handle, const shader_handle& fragment_handle) override;
        shader_handle make_shader(shader_type type, const std::string& source) override;
        vertex_buffer make_vertex_buffer(
            const void* cube_vertices, size_t size, vertex_layout&& layout, buffer_usage use) override;
        index_buffer make_index_buffer(const void* indices, size_t size, index_type type, buffer_usage use) override;

        texture make_texture(void** data, texture_metadata&& metadata, bool free_host_data) override;

        frame_buffer make_frame_buffer(render_texture_data* render_textures, size_t render_texture_count) override;

        void submit(command_list&& commands) override;
        static void reset_gl_state();
        void submit_and_swap(command_list&& commands) override;

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
    };
} // namespace moka
