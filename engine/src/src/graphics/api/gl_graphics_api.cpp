
#include <application/logger.hpp>
#include <application/window.hpp>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <graphics/api/gl_graphics_api.hpp>
#include <graphics/buffer/vertex_layout.hpp>
#include <graphics/command/clear_command.hpp>
#include <graphics/command/command_list.hpp>
#include <graphics/command/draw_command.hpp>
#include <graphics/command/fill_index_buffer_command.hpp>
#include <graphics/command/fill_vertex_buffer_command.hpp>
#include <graphics/command/scissor_command.hpp>
#include <graphics/command/set_material_properties_command.hpp>
#include <graphics/command/viewport_command.hpp>
#include <graphics/material/material.hpp>
#include <string>

namespace moka
{
    constexpr GLenum moka_to_gl(const frame_attachment type)
    {
        switch (type)
        {
        case frame_attachment::color:
            return GL_COLOR_ATTACHMENT0;
        case frame_attachment::depth:
            return GL_DEPTH_ATTACHMENT;
        case frame_attachment::stencil:
            return GL_STENCIL_ATTACHMENT;
        case frame_attachment::depth_stencil:
            return GL_DEPTH_STENCIL_ATTACHMENT;
        default:
            return 0;
        }
    }

    constexpr GLenum moka_to_gl(const frame_format type)
    {
        switch (type)
        {
        case frame_format::depth_component24:
            return GL_DEPTH_COMPONENT24;
        default:
            return 0;
        }
    }

    constexpr GLenum moka_to_gl(const polygon_draw_mode type)
    {
        switch (type)
        {
        case polygon_draw_mode::fill:
            return GL_FILL;
        case polygon_draw_mode::line:
            return GL_LINE;
        case polygon_draw_mode::points:
            return GL_POINTS;
        default:
            return 0;
        }
    }

    constexpr GLenum moka_to_gl(const face type)
    {
        switch (type)
        {
        case face::front:
            return GL_FRONT;
        case face::back:
            return GL_BACK;
        case face::front_and_back:
            return GL_FRONT_AND_BACK;
        default:
            return 0;
        }
    }

    constexpr GLenum moka_to_gl(const primitive_type type)
    {
        switch (type)
        {
        case primitive_type::lines:
            return GL_LINES;
        case primitive_type::line_strip:
            return GL_LINE_STRIP;
        case primitive_type::triangles:
            return GL_TRIANGLES;
        case primitive_type::triangle_strip:
            return GL_TRIANGLE_STRIP;
        default:
            return 0;
        }
    }

    constexpr GLenum moka_to_gl(const index_type type)
    {
        switch (type)
        {
        case index_type::int8:
            return GL_BYTE;
        case index_type::uint8:
            return GL_UNSIGNED_BYTE;
        case index_type::int16:
            return GL_SHORT;
        case index_type::uint16:
            return GL_UNSIGNED_SHORT;
        case index_type::int32:
            return GL_INT;
        case index_type::uint32:
            return GL_UNSIGNED_INT;
        case index_type::float32:
            return GL_FLOAT;
        default:
            return 0;
        }
    }

    constexpr GLenum moka_to_gl(const host_format type)
    {
        switch (type)
        {
        case host_format::rgb:
            return GL_RGB;
        case host_format::rgba:
            return GL_RGBA;
        case host_format::r:
            return GL_R;
        case host_format::rg:
            return GL_RG;
        case host_format::bgr:
            return GL_BGR;
        case host_format::bgra:
            return GL_BGRA;
        default:
            return 0;
        }
    }

    constexpr GLenum moka_to_gl(const device_format type)
    {
        switch (type)
        {
        case device_format::r:
            return GL_R;
        case device_format::rg:
            return GL_RG;
        case device_format::rg16f:
            return GL_RG16F;
        case device_format::rgb:
            return GL_RGB;
        case device_format::srgb8:
            return GL_SRGB8;
        case device_format::rgb16f:
            return GL_RGB16F;
        case device_format::bgr:
            return GL_BGR;
        case device_format::rgba:
            return GL_RGBA;
        case device_format::srgb8_alpha8:
            return GL_SRGB8_ALPHA8;
        case device_format::bgra:
            return GL_BGRA;
        default:
            return 0;
        }
    }

    constexpr GLenum moka_to_gl(const attribute_type type)
    {
        switch (type)
        {
        case attribute_type::boolean:
            return GL_BOOL;
        case attribute_type::int8:
            return GL_BYTE;
        case attribute_type::int16:
            return GL_SHORT;
        case attribute_type::int32:
            return GL_INT;
        case attribute_type::uint8:
            return GL_UNSIGNED_BYTE;
        case attribute_type::uint16:
            return GL_UNSIGNED_SHORT;
        case attribute_type::uint32:
            return GL_UNSIGNED_INT;
        case attribute_type::float16:
            return GL_SHORT;
        case attribute_type::float32:
            return GL_FLOAT;
        case attribute_type::float64:
            return GL_DOUBLE;
        default:
            return 0;
        }
    }

    constexpr GLenum moka_to_gl(const shader_type type)
    {
        switch (type)
        {
        case shader_type::vertex:
            return GL_VERTEX_SHADER;
        case shader_type::fragment:
            return GL_FRAGMENT_SHADER;
        case shader_type::compute:
            return GL_COMPUTE_SHADER;
        default:
            return 0;
        }
    }

    constexpr GLenum moka_to_gl(const blend_equation type)
    {
        switch (type)
        {
        case blend_equation::func_add:
            return GL_FUNC_ADD;
        case blend_equation::func_subtract:
            return GL_FUNC_SUBTRACT;
        case blend_equation::func_reverse_subtract:
            return GL_FUNC_REVERSE_SUBTRACT;
        case blend_equation::min:
            return GL_MIN;
        case blend_equation::max:
            return GL_MAX;
        default:
            return 0;
        }
    }

    constexpr GLenum moka_to_gl(const buffer_usage type)
    {
        switch (type)
        {
        case buffer_usage::static_draw:
            return GL_STATIC_DRAW;
        case buffer_usage::dynamic_draw:
            return GL_DYNAMIC_DRAW;
        case buffer_usage::stream_draw:
            return GL_STREAM_DRAW;
        default:
            return 0;
        }
    }

    constexpr GLenum moka_to_gl(const pixel_type type)
    {
        switch (type)
        {
        case pixel_type::int8:
            return GL_BYTE;
        case pixel_type::int16:
            return GL_SHORT;
        case pixel_type::int32:
            return GL_INT;
        case pixel_type::uint8:
            return GL_UNSIGNED_BYTE;
        case pixel_type::uint16:
            return GL_UNSIGNED_SHORT;
        case pixel_type::uint32:
            return GL_UNSIGNED_INT;
        case pixel_type::float16:
            return GL_HALF_FLOAT;
        case pixel_type::float32:
            return GL_FLOAT;
        default:
            return 0;
        }
    }

    constexpr GLenum moka_to_gl(const texture_target type)
    {
        switch (type)
        {
        case texture_target::texture_1d:
            return GL_TEXTURE_1D;
        case texture_target::texture_2d:
            return GL_TEXTURE_2D;
        case texture_target::texture_3d:
            return GL_TEXTURE_3D;
        case texture_target::array_1d:
            return GL_TEXTURE_1D_ARRAY;
        case texture_target::array_2d:
            return GL_TEXTURE_2D_ARRAY;
        case texture_target::rectangle:
            return GL_TEXTURE_RECTANGLE;
        case texture_target::cubemap:
            return GL_TEXTURE_CUBE_MAP;
        case texture_target::cubemap_array:
            return GL_TEXTURE_CUBE_MAP_ARRAY;
        case texture_target::texture_buffer:
            return GL_TEXTURE_BUFFER;
        case texture_target::texture_2d_multisample:
            return GL_TEXTURE_2D_MULTISAMPLE;
        case texture_target::texture_2d_multisample_array:
            return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
        default:
            return 0;
        }
    }

    constexpr GLenum moka_to_gl(const image_target type)
    {
        switch (type)
        {
        case image_target::texture_2d:
            return GL_TEXTURE_2D;
        case image_target::texture_2d_proxy:
            return GL_PROXY_TEXTURE_2D;
        case image_target::array_1d:
            return GL_TEXTURE_1D_ARRAY;
        case image_target::array_1d_proxy:
            return GL_PROXY_TEXTURE_1D_ARRAY;
        case image_target::rectangle:
            return GL_TEXTURE_RECTANGLE;
        case image_target::rectangle_proxy:
            return GL_PROXY_TEXTURE_2D;
        case image_target::cubemap_positive_x:
            return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
        case image_target::cubemap_positive_y:
            return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
        case image_target::cubemap_positive_z:
            return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
        case image_target::cubemap_negative_x:
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
        case image_target::cubemap_negative_y:
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
        case image_target::cubemap_negative_z:
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
        case image_target::cubemap_proxy:
            return GL_PROXY_TEXTURE_CUBE_MAP;
        default:
            return 0;
        }
    }

    constexpr GLenum moka_to_gl(const blend_function_factor type)
    {
        switch (type)
        {
        case blend_function_factor::zero:
            return GL_ZERO;
        case blend_function_factor::one:
            return GL_ONE;
        case blend_function_factor::src_color:
            return GL_SRC_COLOR;
        case blend_function_factor::one_minus_src_color:
            return GL_ONE_MINUS_SRC_COLOR;
        case blend_function_factor::dst_color:
            return GL_DST_COLOR;
        case blend_function_factor::one_minus_dst_color:
            return GL_ONE_MINUS_DST_COLOR;
        case blend_function_factor::src_alpha:
            return GL_SRC_ALPHA;
        case blend_function_factor::one_minus_src_alpha:
            return GL_ONE_MINUS_SRC_ALPHA;
        case blend_function_factor::dst_alpha:
            return GL_DST_ALPHA;
        case blend_function_factor::one_minus_dst_alpha:
            return GL_ONE_MINUS_DST_ALPHA;
        case blend_function_factor::constant_color:
            return GL_CONSTANT_COLOR;
        case blend_function_factor::one_minus_constant_color:
            return GL_ONE_MINUS_CONSTANT_COLOR;
        case blend_function_factor::constant_alpha:
            return GL_CONSTANT_ALPHA;
        case blend_function_factor::one_minus_constant_alpha:
            return GL_ONE_MINUS_CONSTANT_ALPHA;
        default:
            return 0;
        }
    }

    void gl_graphics_api::visit(clear_command& cmd)
    {
        uint32_t bitmask = 0;

        if (cmd.clear_color)
        {
            auto& color = cmd.color;
            glClearColor(color.r, color.g, color.b, color.a);
            bitmask = bitmask | GL_COLOR_BUFFER_BIT;
        }

        if (cmd.clear_depth)
        {
            bitmask = bitmask | GL_DEPTH_BUFFER_BIT;
        }

        glClear(bitmask);

        if constexpr (application_traits::is_debug_build)
        {
            check_errors("visit clear_command");
        }
    }

    void gl_graphics_api::visit(viewport_command& cmd)
    {
        glViewport(cmd.x, cmd.y, cmd.width, cmd.height);

        if constexpr (application_traits::is_debug_build)
        {
            check_errors("visit viewport_command");
        }
    }

    void gl_graphics_api::visit(scissor_command& cmd)
    {
        glScissor(cmd.x, cmd.y, cmd.width, cmd.height);

        if constexpr (application_traits::is_debug_build)
        {
            check_errors("visit scissor_command");
        }
    }

    void gl_graphics_api::visit(fill_vertex_buffer_command& cmd)
    {
        const GLuint handle(cmd.handle.id);

        auto& data = vertex_buffer_data_[cmd.handle.id];
        data.size = cmd.size;

        glBindBuffer(GL_ARRAY_BUFFER, handle);
        glBufferData(GL_ARRAY_BUFFER, cmd.size, cmd.data, moka_to_gl(data.buffer_use));
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        if constexpr (application_traits::is_debug_build)
        {
            check_errors("visit fill_vertex_buffer_command");
        }
    }

    void gl_graphics_api::visit(fill_index_buffer_command& cmd)
    {
        const GLuint handle(cmd.handle.id);

        auto& data = index_buffer_data_[cmd.handle.id];
        data.size = cmd.size;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, cmd.size, cmd.data, moka_to_gl(data.buffer_use));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        if constexpr (application_traits::is_debug_build)
        {
            check_errors("visit fill_index_buffer_command");
        }
    }

    void gl_graphics_api::check_errors(const char* caller)
    {
        // check OpenGL error
        while (glGetError() != GL_NO_ERROR)
        {
            log_.error("OpenGL error discovered. Last call: {}", caller);
        }
    }

    void gl_graphics_api::visit(frame_buffer_command& cmd)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, GLuint{cmd.buffer.id});

        if constexpr (application_traits::is_debug_build)
        {
            check_errors("visit frame_buffer_command");
        }
    }

    void gl_graphics_api::visit(frame_buffer_texture_command& cmd)
    {
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, moka_to_gl(cmd.attachment), moka_to_gl(cmd.target), GLuint{cmd.texture.id}, cmd.level);

        if constexpr (application_traits::is_debug_build)
        {
            check_errors("visit frame_buffer_texture_command");
        }
    }

    void gl_graphics_api::visit(generate_mipmaps_command& cmd)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, cmd.texture.id);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }

    void gl_graphics_api::visit(set_material_parameters_command& cmd)
    {
        auto& cache = device_.get_material_cache();

        auto* material = cache.get_material(cmd.material);

        if (material)
        {
            for (const auto& parameter : cmd.parameters)
            {
                (*material)[parameter.name] = parameter;
            }
        }
    }

    void gl_graphics_api::visit(draw_command& cmd)
    {
        // early exit if a bogus vertex buffer is given to us
        if (cmd.vertex_buffer.id == std::numeric_limits<uint16_t>::max())
        {
            return;
        }

        const auto indexed = cmd.index_buffer.id != std::numeric_limits<uint16_t>::max();

        glBindBuffer(GL_ARRAY_BUFFER, cmd.vertex_buffer.id);

        if (indexed)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cmd.index_buffer.id);
        }

        // update the current vertex layout to match the vertices we're about to render
        auto& layout = vertex_buffer_data_[cmd.vertex_buffer.id].layout;

        for (const auto& attribute : layout)
        {
            glVertexAttribPointer(
                GLuint(attribute.index),
                GLint(attribute.size),
                moka_to_gl(attribute.type),
                attribute.normalized,
                GLsizei(attribute.stride),
                reinterpret_cast<void*>(attribute.offset));

            glEnableVertexAttribArray(GLuint(attribute.index));
        }

        glEnableVertexAttribArray(0);

        auto& material_handle = cmd.material;
        auto& previous_handle = previous_command_.material;

        auto& material_cache = device_.get_material_cache();
        auto* material = material_cache.get_material(material_handle);
        auto* previous = material_cache.get_material(previous_handle);

        if (material)
        {
            if (previous)
            {
                // if this is a different shader, update the program state
                if (material->get_program().id != previous->get_program().id)
                {
                    glUseProgram(material->get_program().id);
                }
            }
            else
            {
                glUseProgram(material->get_program().id);
            }

            auto& blend = material->get_blend();
            blend.enabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
            glBlendEquation(moka_to_gl(blend.equation));
            glBlendFunc(moka_to_gl(blend.source), moka_to_gl(blend.destination));

            auto& polygon_mode = material->get_polygon_mode();
            glPolygonMode(moka_to_gl(polygon_mode.faces), moka_to_gl(polygon_mode.mode));

            auto& culling = material->get_culling();
            culling.enabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
            glCullFace(moka_to_gl(culling.faces));

            material->get_depth_test() ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
            material->get_scissor_test() ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST);

            const auto size = material->size();

            size_t current_texture_unit = 0;

            for (size_t i = 0; i < size; i++)
            {
                auto& parameter = (*material)[i];

                const auto location =
                    glGetUniformLocation(GLuint(material->get_program().id), parameter.name.c_str());

                if (location == -1)
                    continue;

                switch (parameter.type)
                {
                case parameter_type::texture:
                {
                    const auto data = std::get<texture_handle>(parameter.data);
                    glUniform1i(location, GLint(current_texture_unit));
                    glActiveTexture(GL_TEXTURE0 + GLenum(current_texture_unit));

                    auto& meta_data = texture_data_[data.id];

                    glBindTexture(moka_to_gl(meta_data.target), GLuint(data.id));

                    ++current_texture_unit;
                    break;
                }
                case parameter_type::float32:
                {
                    auto data = std::get<float>(parameter.data);
                    glUniform1fv(location, GLsizei(parameter.count), &data);
                    break;
                }
                case parameter_type::vec3:
                {
                    auto data = std::get<glm::vec3>(parameter.data);
                    glUniform3fv(location, GLsizei(parameter.count), glm::value_ptr(data));
                    break;
                }
                case parameter_type::vec4:
                {
                    auto data = std::get<glm::vec4>(parameter.data);
                    glUniform4fv(location, GLsizei(parameter.count), glm::value_ptr(data));
                    break;
                }
                case parameter_type::mat3:
                {
                    auto data = std::get<glm::mat3>(parameter.data);
                    glUniformMatrix3fv(location, GLsizei(parameter.count), false, glm::value_ptr(data));
                    break;
                }
                case parameter_type::mat4:
                {
                    auto data = std::get<glm::mat4>(parameter.data);
                    glUniformMatrix4fv(location, GLsizei(parameter.count), false, glm::value_ptr(data));
                    break;
                }
                default:;
                }
            }
        }

        if (indexed)
        {
            glDrawElements(
                moka_to_gl(cmd.prim_type),
                GLsizei(cmd.index_count),
                moka_to_gl(cmd.idx_type),
                reinterpret_cast<void*>(cmd.index_buffer_offset));
        }
        else
        {
            glDrawArrays(moka_to_gl(cmd.prim_type), GLint(cmd.first_vertex), GLsizei(cmd.vertex_count));
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        previous_command_ = cmd;

        if constexpr (application_traits::is_debug_build)
        {
            check_errors("visit draw_command");
        }
    }

    program_handle gl_graphics_api::make_program(const shader_handle& vertex_handle, const shader_handle& fragment_handle)
    {
        const auto id = glCreateProgram();

        const program_handle result{static_cast<uint16_t>(id)};

        // Attach shaders as necessary.

        glAttachShader(id, vertex_handle.id);

        glAttachShader(id, fragment_handle.id);

        // Link the program.
        glLinkProgram(id);

        auto is_linked = 0;

        glGetProgramiv(id, GL_LINK_STATUS, &is_linked);
        if (is_linked == GL_FALSE)
        {
            auto max_length = 0;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &max_length);

            // The maxLength includes the NULL character
            char info_log[512];

            glGetProgramInfoLog(id, max_length, &max_length, &info_log[0]);

            // The program is useless now. So delete it.
            glDeleteProgram(id);

            glGetShaderInfoLog(id, 512, nullptr, info_log);

            log_.info(info_log);
        }

        if constexpr (application_traits::is_debug_build)
        {
            check_errors("make_program");
        }

        return result;
    }

    shader_handle gl_graphics_api::make_shader(const shader_type type, const std::string& source)
    {
        int success;

        char info_log[512];

        const auto id = glCreateShader(moka_to_gl(type));

        const shader_handle result{static_cast<uint16_t>(id)};

        auto source_chars = source.c_str();

        glShaderSource(id, 1, &source_chars, nullptr);

        glCompileShader(id);

        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            using namespace std::string_literals;

            glGetShaderInfoLog(id, 512, nullptr, info_log);

            log_.error(info_log);
        }

        if constexpr (application_traits::is_debug_build)
        {
            check_errors("make_shader");
        }

        return result;
    }

    vertex_buffer_handle gl_graphics_api::make_vertex_buffer(
        const void* vertices, const size_t size, vertex_layout&& layout, const buffer_usage use)
    {
        vertex_buffer_handle result;

        GLuint handle;

        vertex_metadata data;
        data.layout = std::move(layout);
        data.buffer_use = use;
        data.size = size;

        glGenBuffers(1, &handle);

        result.id = uint16_t(handle);

        vertex_buffer_data_[result.id] = std::move(data);

        glBindBuffer(GL_ARRAY_BUFFER, handle);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, moka_to_gl(use));
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        if constexpr (application_traits::is_debug_build)
        {
            check_errors("make_vertex_buffer");
        }

        return result;
    }

    index_buffer_handle gl_graphics_api::make_index_buffer(
        const void* indices, const size_t size, const index_type type, const buffer_usage use)
    {
        index_buffer_handle result;

        GLuint handle;

        index_metadata data;
        data.buffer_use = use;
        data.size = size;
        data.type = type;

        glGenBuffers(1, &handle);

        result.id = uint16_t(handle);

        index_buffer_data_[result.id] = data;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, moka_to_gl(use));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        if constexpr (application_traits::is_debug_build)
        {
            check_errors("make_index_buffer");
        }

        return result;
    }

    void gl_graphics_api::submit(command_list&& commands)
    {
        commands.accept(*this);

        reset_gl_state();
    }

    void gl_graphics_api::reset_gl_state()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDrawBuffer(GL_BACK);
    }

    void gl_graphics_api::submit_and_swap(command_list&& commands)
    {
        commands.accept(*this);
        window_.swap_buffer();

        reset_gl_state();

        previous_command_ = {};
    }

    constexpr GLenum moka_to_gl(const wrap_mode type)
    {
        switch (type)
        {
        case wrap_mode::clamp_to_edge:
            return GL_CLAMP_TO_EDGE;
        case wrap_mode::mirrored_repeat:
            return GL_MIRRORED_REPEAT;
        case wrap_mode::repeat:
            return GL_REPEAT;
        default:
            return 0;
        }
    }

    constexpr GLenum moka_to_gl(const min_filter type)
    {
        switch (type)
        {
        case min_filter::nearest:
            return GL_NEAREST;
        case min_filter::linear:
            return GL_LINEAR;
        case min_filter::nearest_mipmap_nearest:
            return GL_NEAREST_MIPMAP_NEAREST;
        case min_filter::linear_mipmap_nearest:
            return GL_LINEAR_MIPMAP_NEAREST;
        case min_filter::nearest_mipmap_linear:
            return GL_NEAREST_MIPMAP_LINEAR;
        case min_filter::linear_mipmap_linear:
            return GL_LINEAR_MIPMAP_LINEAR;
        default:
            return 0;
        }
    }

    constexpr GLenum moka_to_gl(const mag_filter type)
    {
        switch (type)
        {
        case mag_filter::nearest:
            return GL_NEAREST;
        case mag_filter::linear:
            return GL_LINEAR;
        default:
            return 0;
        }
    }

    texture_handle gl_graphics_api::make_texture(void** data, texture_metadata&& metadata, const bool free_host_data)
    {
        const auto gl_target = moka_to_gl(metadata.target);

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(gl_target, texture);

        for (size_t i = 0; i < metadata.data.size(); i++)
        {
            const auto pixels = data[i];
            const auto& tex_image = metadata.data[i];

            glTexImage2D(
                moka_to_gl(metadata.data[i].target),
                tex_image.mip_level,
                moka_to_gl(tex_image.internal_format),
                tex_image.width,
                tex_image.height,
                tex_image.border,
                moka_to_gl(tex_image.base_format),
                moka_to_gl(tex_image.type),
                pixels);
        }

        glTexParameteri(gl_target, GL_TEXTURE_WRAP_S, moka_to_gl(metadata.wrap_mode.s));
        glTexParameteri(gl_target, GL_TEXTURE_WRAP_T, moka_to_gl(metadata.wrap_mode.t));
        glTexParameteri(gl_target, GL_TEXTURE_WRAP_R, moka_to_gl(metadata.wrap_mode.r));
        glTexParameteri(gl_target, GL_TEXTURE_MIN_FILTER, moka_to_gl(metadata.filter_mode.min));
        glTexParameteri(gl_target, GL_TEXTURE_MAG_FILTER, moka_to_gl(metadata.filter_mode.mag));

        if (metadata.generate_mipmaps)
        {
            glGenerateMipmap(gl_target);
        }

        if constexpr (application_traits::is_debug_build)
        {
            check_errors("make_texture");
        }

        const auto id = static_cast<uint16_t>(texture);

        texture_data_[id] = std::move(metadata);

        return moka::texture_handle{id};
    }

    frame_buffer_handle gl_graphics_api::make_frame_buffer(render_texture_data* render_textures, const size_t render_texture_count)
    {
        unsigned int capture_fbo;
        glGenFramebuffers(1, &capture_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo);

        for (size_t i = 0; i < render_texture_count; i++)
        {
            auto& render_texture = render_textures[i];

            unsigned int capture_rbo;
            glGenRenderbuffers(1, &capture_rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo);

            glRenderbufferStorage(
                GL_RENDERBUFFER,
                moka_to_gl(render_texture.format),
                render_texture.width,
                render_texture.height);

            glFramebufferRenderbuffer(
                GL_FRAMEBUFFER, moka_to_gl(render_texture.attachment), GL_RENDERBUFFER, capture_rbo);
        }

        if constexpr (application_traits::is_debug_build)
        {
            check_errors("make_frame_buffer");
        }

        return frame_buffer_handle{static_cast<uint16_t>(capture_fbo)};
    }

    void GLAPIENTRY gl_graphics_api::message_callback(
        const GLenum source,
        const GLenum type,
        const GLuint id,
        const GLenum severity,
        const GLsizei length,
        const GLchar* message,
        const void* user_param)
    {
        std::string msg_source;
        switch (source)
        {
        case GL_DEBUG_SOURCE_API:
            msg_source = "API";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            msg_source = "SHADER_COMPILER";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            msg_source = "THIRD_PARTY";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            msg_source = "APPLICATION";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            msg_source = "OTHER";
            break;
        default:;
        }

        std::string msg_type;
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:
            msg_type = "ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            msg_type = "DEPRECATED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            msg_type = "UNDEFINED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            msg_type = "PORTABILITY";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            msg_type = "PERFORMANCE";
            break;
        case GL_DEBUG_TYPE_OTHER:
            msg_type = "OTHER";
            break;
        default:;
        }

        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            log_.error("message: {}, type: {}, source: {}", message, msg_type.c_str(), msg_source.c_str());
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            log_.warn("message: {}, type: {}, source: {}", message, msg_type.c_str(), msg_source.c_str());
            break;
        case GL_DEBUG_SEVERITY_LOW:
            log_.debug("message: {}, type: {}, source: {}", message, msg_type.c_str(), msg_source.c_str());
            break;
        default:
            log_.trace(
                "glDebugMessage: {}, type: {}, source: {}", message, msg_type.c_str(), msg_source.c_str());
            break;
        }
    }

    logger gl_graphics_api::log_("OpenGL");

    gl_graphics_api::gl_graphics_api(window& window, graphics_device& device)
        : window_(window), device_(device)
    {
        // enable GL_DEBUG_OUTPUT by default if it's a debug build
        if constexpr (application_traits::is_debug_build)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glDebugMessageCallback(message_callback, nullptr);
        }

        glGenVertexArrays(1, &vao_);
        glBindVertexArray(vao_);

        glEnable(GL_MULTISAMPLE);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

        glDepthFunc(GL_LEQUAL);

        if constexpr (application_traits::is_debug_build)
        {
            check_errors("gl_graphics_api");
        }
    }

    gl_graphics_api::~gl_graphics_api()
    {
        glDeleteVertexArrays(1, &vao_);

        if constexpr (application_traits::is_debug_build)
        {
            check_errors("~gl_graphics_api");
        }
    }
} // namespace moka
