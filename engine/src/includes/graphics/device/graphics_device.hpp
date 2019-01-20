#pragma once

#include "graphics/buffer/frame_buffer.hpp"
#include <application/window.hpp>
#include <graphics/api/graphics_api.hpp>
#include <graphics/command/command_list.hpp>
#include <memory>

namespace moka
{
    enum class graphics_backend
    {
        direct3d_9,
        direct3d_11,
        direct3d_12,
        gnm,
        metal,
        opengl_es,
        opengl,
        vulkan,
        null
    };

    class graphics_device
    {
        std::unique_ptr<graphics_api> graphics_api_;

    public:
        explicit graphics_device(window& window, graphics_backend graphics_backend = graphics_backend::opengl);

        vertex_buffer make_vertex_buffer(
            const void* vertices, size_t size, vertex_layout&& layout, buffer_usage use) const;

        index_buffer make_index_buffer(const void* indices, size_t size, index_type type, buffer_usage use) const;

        shader make_shader(shader_type type, const std::string& source) const;

        program make_program(shader vertex_handle, shader fragment_handle) const;

        texture make_texture(void** data, texture_metadata&& metadata, bool free_host_data) const;

        texture_builder build_texture();

        frame_buffer make_frame_buffer(render_texture_data* render_textures, size_t render_texture_count);

        frame_buffer_builder build_frame_buffer();

        void destroy(program handle);

        void destroy(shader handle);

        void destroy(vertex_buffer handle);

        void destroy(index_buffer handle);

        void submit(command_list&& command_list, bool sort = true) const;

        void submit_and_swap(command_list&& command_list, bool sort = true) const;
    };
} // namespace moka
