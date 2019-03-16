
#include <application/window.hpp>
#include <graphics/api/gl_graphics_api.hpp>
#include <graphics/device/graphics_device.hpp>

namespace moka
{
    texture_cache& graphics_device::get_texture_cache()
    {
        return textures_;
    }

    const texture_cache& graphics_device::get_texture_cache() const
    {
        return textures_;
    }

    program_cache& graphics_device::get_shader_cache()
    {
        return shaders_;
    }

    const program_cache& graphics_device::get_shader_cache() const
    {
        return shaders_;
    }

    material_cache& graphics_device::get_material_cache()
    {
        return materials_;
    }

    const material_cache& graphics_device::get_material_cache() const
    {
        return materials_;
    }

    graphics_device::graphics_device(window& window, const graphics_backend graphics_backend)
        : textures_(*this), shaders_(*this), materials_(*this)
    {
        auto context = window.make_context();

        // right now we only have an OpenGL backend!
        switch (graphics_backend)
        {
        case graphics_backend::direct3d_9:
            graphics_api_ = std::make_unique<gl_graphics_api>(window, *this);
            break;
        case graphics_backend::direct3d_11:
            graphics_api_ = std::make_unique<gl_graphics_api>(window, *this);
            break;
        case graphics_backend::direct3d_12:
            graphics_api_ = std::make_unique<gl_graphics_api>(window, *this);
            break;
        case graphics_backend::gnm:
            graphics_api_ = std::make_unique<gl_graphics_api>(window, *this);
            break;
        case graphics_backend::metal:
            graphics_api_ = std::make_unique<gl_graphics_api>(window, *this);
            break;
        case graphics_backend::opengl_es:
            graphics_api_ = std::make_unique<gl_graphics_api>(window, *this);
            break;
        case graphics_backend::opengl:
            graphics_api_ = std::make_unique<gl_graphics_api>(window, *this);
            break;
        case graphics_backend::vulkan:
            graphics_api_ = std::make_unique<gl_graphics_api>(window, *this);
            break;
        case graphics_backend::null:
            graphics_api_ = nullptr;
            break;
        default:
            graphics_api_ = nullptr;
        }
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

    vertex_buffer_handle graphics_device::make_vertex_buffer(
        const void* cube_vertices, const size_t size, vertex_layout&& layout, const buffer_usage use) const
    {
        return graphics_api_->make_vertex_buffer(cube_vertices, size, std::move(layout), use);
    }

    index_buffer_handle graphics_device::make_index_buffer(
        const void* indices, const size_t size, const index_type type, const buffer_usage use) const
    {
        return graphics_api_->make_index_buffer(indices, size, type, use);
    }

    shader_handle graphics_device::make_shader(const shader_type type, const std::string& source) const
    {
        return graphics_api_->make_shader(type, source);
    }

    program_handle graphics_device::make_program(const shader_handle vertex_handle, const shader_handle fragment_handle) const
    {
        return graphics_api_->make_program(vertex_handle, fragment_handle);
    }

    texture_handle graphics_device::make_texture(void** data, texture_metadata&& metadata, const bool free_host_data) const
    {
        const auto size = metadata.data.size();

        const auto handle = graphics_api_->make_texture(data, std::move(metadata), free_host_data);

        // this does not seem very safe at all! what if make_texture throws! Leak city!
        if (free_host_data)
        {
            for (size_t i = 0; i < size; i++)
            {
                free_texture(data[i]);
            }
        }

        return handle;
    }

    texture_builder graphics_device::build_texture()
    {
        return texture_builder{*this};
    }

    frame_buffer_handle graphics_device::make_frame_buffer(render_texture_data* render_textures, const size_t render_texture_count) const
    {
        return graphics_api_->make_frame_buffer(render_textures, render_texture_count);
    }

    frame_buffer_builder graphics_device::build_frame_buffer()
    {
        return frame_buffer_builder{*this};
    }

    material_builder graphics_device::build_material()
    {
        return material_builder{*this};
    }

    void graphics_device::destroy(program_handle handle)
    {
    }

    void graphics_device::destroy(vertex_buffer_handle handle)
    {
    }

    void graphics_device::destroy(index_buffer_handle handle)
    {
    }

    void graphics_device::destroy(const shader_handle handle)
    {
    }
} // namespace moka
