
#include "application/profile.hpp"
#include <application/window.hpp>
#include <graphics/api/gl_graphics_api.hpp>
#include <graphics/device/graphics_device.hpp>

namespace moka
{
    std::unique_ptr<graphics_api> create(window& window, const graphics_backend graphics_backend)
    {
        auto context = window.make_context();

        // right now we only have an OpenGL backend!
        switch (graphics_backend)
        {
        case graphics_backend::direct3d_9:
            return std::make_unique<gl_graphics_api>(window);
        case graphics_backend::direct3d_11:
            return std::make_unique<gl_graphics_api>(window);
        case graphics_backend::direct3d_12:
            return std::make_unique<gl_graphics_api>(window);
        case graphics_backend::gnm:
            return std::make_unique<gl_graphics_api>(window);
        case graphics_backend::metal:
            return std::make_unique<gl_graphics_api>(window);
        case graphics_backend::opengl_es:
            return std::make_unique<gl_graphics_api>(window);
        case graphics_backend::opengl:
            return std::make_unique<gl_graphics_api>(window);
        case graphics_backend::vulkan:
            return std::make_unique<gl_graphics_api>(window);
        case graphics_backend::null:
            return nullptr;
        default:
            return std::make_unique<gl_graphics_api>(window);
        }
    }

    graphics_device::graphics_device(window& window, const graphics_backend graphics_backend)
        : graphics_api_(create(window, graphics_backend))
    {
    }

    void graphics_device::submit(command_list&& command_list, bool sort) const
    {
        if (sort && !command_list.is_sorted())
        {
            command_list.sort();
        }

        graphics_api_->submit(std::move(command_list));
    }

    void graphics_device::submit_and_swap(command_list&& command_list, bool sort) const
    {
        if (sort && !command_list.is_sorted())
        {
            command_list.sort();
        }

        graphics_api_->submit_and_swap(std::move(command_list));
    }

    vertex_buffer graphics_device::make_vertex_buffer(
        const void* vertices, const size_t size, vertex_layout&& layout, const buffer_usage use) const
    {
        return graphics_api_->make_vertex_buffer(vertices, size, std::move(layout), use);
    }

    index_buffer graphics_device::make_index_buffer(
        const void* indices, const size_t size, const index_type type, const buffer_usage use) const
    {
        return graphics_api_->make_index_buffer(indices, size, type, use);
    }

    shader graphics_device::make_shader(const shader_type type, const std::string& source) const
    {
        return graphics_api_->make_shader(type, source);
    }

    program graphics_device::make_program(const shader vertex_handle, const shader fragment_handle) const
    {
        return graphics_api_->make_program(vertex_handle, fragment_handle);
    }

    texture graphics_device::make_texture(
        void* data,
        const glm::ivec2& resolution,
        texture_components components,
        texture_wrap_mode wrap_mode,
        bool has_mipmaps,
        bool free_data) const
    {
        auto handle = graphics_api_->make_texture(
            data, resolution, components, wrap_mode, has_mipmaps);

        if (free_data)
        {
            moka::free_texture(data);
        }

        return handle;
    }

    void graphics_device::destroy(program handle)
    {
    }

    void graphics_device::destroy(vertex_buffer handle)
    {
    }

    void graphics_device::destroy(index_buffer handle)
    {
    }

    void graphics_device::destroy(const shader handle)
    {
    }
} // namespace moka
