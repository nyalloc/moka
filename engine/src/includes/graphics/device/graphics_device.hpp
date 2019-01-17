#pragma once

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
        graphics_device(window& window, graphics_backend graphics_backend = graphics_backend::opengl);

        vertex_buffer make_vertex_buffer(
            const void* vertices, size_t size, vertex_layout&& layout, buffer_usage use) const;

        index_buffer make_index_buffer(const void* indices, size_t size, index_type type, buffer_usage use) const;

        shader make_shader(shader_type type, const std::string& source) const;

        program make_program(shader vertex_handle, shader fragment_handle) const;

        texture make_texture(
            texture_target target,
            void* pixels,
            texture_type type,
            int width,
            int height,
            base_pixel_format base_format,
            internal_pixel_format internal_format,
            texture_filter_mode filter_mode,
            texture_wrap_mode wrap_mode,
            bool has_mipmaps,
            bool free_memory) const;

        void destroy(program handle);

        void destroy(shader handle);

        void destroy(vertex_buffer handle);

        void destroy(index_buffer handle);

        void submit(command_list&& command_list, bool sort = true) const;

        void submit_and_swap(command_list&& command_list, bool sort = true) const;
    };
} // namespace moka
