/*
===========================================================================
Moka Source Code
Copyright 2019 Stuart Adams. All rights reserved.
https://github.com/stuartdadams/moka
stuartdadams | linkedin.com/in/stuartdadams

This file is part of the Moka Real-Time Physically-Based Rendering Project.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

===========================================================================
*/
#pragma once

#include "graphics/buffer/frame_buffer_handle.hpp"
#include "graphics/material/material_parameter.hpp"
#include <asset_importer/texture_importer.hpp>
#include <graphics/buffer/buffer_usage.hpp>
#include <graphics/buffer/index_buffer_handle.hpp>
#include <graphics/buffer/vertex_buffer_handle.hpp>
#include <graphics/buffer/vertex_layout.hpp>
#include <graphics/device/graphics_visitor.hpp>
#include <graphics/program.hpp>
#include <graphics/shader.hpp>
#include <graphics/texture_handle.hpp>

namespace moka
{
    class vertex_layout;
    class command_list;

    /**
     * \brief
     */
    enum class alpha_mode : uint8_t
    {
        blend, // The rendered output is combined with the background using the normal painting operation(i.e.the Porter and Duff over operator). This mode is used to simulate geometry such as guaze cloth or animal fur.
        mask, // The rendered output is either fully opaque or fully transparent depending on the alpha value and the specified alpha cutoff value. This mode is used to simulate geometry such as tree leaves or wire fences.
        opaque // The rendered output is fully opaque and any alpha value is ignored.
    };

    enum class attribute
    {
        position,
        normal,
        tangent,
        bitangent,
        color0,
        color1,
        color2,
        color3,
        indices,
        weight,
        tex_coord0,
        tex_coord1,
        tex_coord2,
        tex_coord3,
        tex_coord4,
        tex_coord5,
        tex_coord6,
        tex_coord7,
        extension
    };

    template <typename T>
    struct basic_rectangle
    {
        T x, y, width, height;

        constexpr basic_rectangle(const T x, const T y, const T width, const T height) noexcept
            : x{x}, y{y}, width{width}, height{height}
        {
        }
    };

    using rectangle = basic_rectangle<int>;

    enum class primitive_type : uint8_t
    {
        points,
        lines,
        line_loop,
        line_strip,
        triangles,
        triangle_strip,
        triangle_fan
    };

    enum class toggle : uint8_t
    {
        enable,
        disable
    };

    enum class cull_face : uint8_t
    {
        front,
        back,
        front_and_back
    };

    enum class blend_function_factor : uint8_t
    {
        zero,
        one,
        src_color,
        one_minus_src_color,
        dst_color,
        one_minus_dst_color,
        src_alpha,
        one_minus_src_alpha,
        dst_alpha,
        one_minus_dst_alpha,
        constant_color,
        one_minus_constant_color,
        constant_alpha,
        one_minus_constant_alpha
    };

    enum class blend_equation : uint8_t
    {
        func_add,
        func_subtract,
        func_reverse_subtract,
        min,
        max
    };

    enum class face : uint8_t
    {
        front,
        back,
        front_and_back
    };

    struct uniform_data
    {
        std::string name;
        parameter_type type;
        size_t count;
        size_t buffer_start;
        size_t buffer_end;
    };

    struct texture_binding
    {
        texture_handle handle;
        size_t unit;
    };

    struct draw_call;

    /**
     * \brief render_context abstracts the native rendering API.
     */
    class graphics_api : public graphics_visitor
    {
    public:
        virtual ~graphics_api() = default;

        graphics_api() = default;

        graphics_api(const graphics_api& rhs) = default;

        graphics_api(graphics_api&& rhs) = default;

        graphics_api& operator=(const graphics_api& rhs) = default;

        graphics_api& operator=(graphics_api&& rhs) = default;

        /**
         * \brief Submit a command_list to execute on the device.
         * \param commands The command_list you wish to run.
         */
        virtual void submit(command_list&& commands) = 0;

        /**
         * \brief Submit a command_list to execute on the device. The main framebuffer will be swapped after executing, advancing a frame.
         * \param commands The command_list you wish to run.
         */
        virtual void submit_and_swap(command_list&& commands) = 0;

        /**
         * \brief Create a new frame buffer.
         * \param render_textures An array of render_texture_data.
         * \param render_texture_count Size of the render_textures array.
         * \return A new frame_buffer_handle representing a frame buffer on the device.
         */
        virtual frame_buffer_handle make_frame_buffer(
            render_texture_data* render_textures, size_t render_texture_count) = 0;

        /**
         * \brief Create a shader program from vertex & fragment shaders.
         * \param vertex_handle The vertex shader that you want to link to this program.
         * \param fragment_handle The fragment shader that you want to link to this program.
         * \return A new program_handle representing a program on the device.
         */
        virtual program_handle make_program(
            const shader_handle& vertex_handle, const shader_handle& fragment_handle) = 0;

        /**
         * \brief Create a shader from source code.
         * \param type The type of shader you want to create.
         * \param source The source code of the shader you want to create.
         * \return A new shader_handle representing a shader on the device.
         */
        virtual shader_handle make_shader(shader_type type, const std::string& source) = 0;

        /**
         * \brief Create a new vertex buffer.
         * \param vertices The host memory buffer that will be used as vertex data.
         * \param size The size of the host vertex buffer.
         * \param layout The layout of the vertex data.
         * \param use A buffer usage hint.
         * \return A new vertex_buffer_handle representing an vertex buffer on the device.
         */
        virtual vertex_buffer_handle make_vertex_buffer(
            const void* vertices, size_t size, vertex_layout&& layout, buffer_usage use) = 0;

        /**
         * \brief Create a new index buffer.
         * \param indices The host memory buffer that will be used as index data.
         * \param size The size of the host index buffer.
         * \param type The layout of the index data.
         * \param use A buffer usage hint.
         * \return A new index_buffer_handle representing an index buffer on the device.
         */
        virtual index_buffer_handle make_index_buffer(
            const void* indices, size_t size, index_type type, buffer_usage use) = 0;

        /**
         * \brief Create a new texture.
         * \param data The host memory buffer that will be used as texture data.
         * \param metadata Metadata describing the texture data.
         * \param free_host_data If true, free the host memory after uploading to the device. Otherwise allow the calling code to free it.
         * \return A new texture_handle representing a texture on the device.
         */
        virtual texture_handle make_texture(
            const void** data, texture_metadata&& metadata, bool free_host_data) = 0;

        /**
         * \brief Destroy a frame buffer.
         * \param handle The host frame buffer that will be destroyed.
         */
        virtual void destroy(frame_buffer_handle handle) = 0;
    };
} // namespace moka
