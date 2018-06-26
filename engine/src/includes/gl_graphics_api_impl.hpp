#pragma once

#include <graphics_api_impl.hpp>

namespace neon
{
    /**
     * \brief Convert the interface of OpenGL into the neon rendring API.
     * Allows neon rendering functionality to work at a higher level without being coupled to an OpenGL backend.
     */
    class gl_graphics_api_impl : public graphics_api_impl
    {
        logger log_;
        std::vector<shader_handle> shaders_;
        std::vector<program_handle> programs_;
        std::vector<vertex_buffer_handle> vertex_buffers_;
    public:
        gl_graphics_api_impl();
        std::string version() const override;
        std::string vendor() const override;
        std::string renderer() const override;
        std::string extensions() const override;
        std::string shading_language_version() const override;
        void clear(bool color, bool depth, bool stencil, byte stencil_value, const colour& colour) const override;
        void depth_bounds_test(float zmin, float zmax) const override;
        void polygon_offset(float scale, float bias) const override;
        void check_errors() const override;
        void clear_colour(const colour& colour) const override;
        void set_scissor(const rectangle& bounds) const override;
        void set_viewport(const rectangle& bounds) const override;
        void draw_indexed_primitives(primitive_type primitive_type, size_t index_count) const override;
        void draw_primatives(primitive_type primitive_type, size_t vertex_count) const override;
        void scissor_test(toggle toggle) const override;
        void depth_mask(toggle toggle) const override;
        void depth_test(toggle toggle) const override;
        void face_culling(toggle toggle) const override;
        void blend(toggle toggle) const override;
        void blend_function(blend_function_factor lhs, blend_function_factor rhs) const override;
        program_handle create_program(const shader_handle& vertex_handle, const shader_handle& fragment_handle) override;
        void destroy(const shader_handle& handle) override;
        shader_handle create_shader(shader_type type, const std::string& source) override;
        vertex_buffer_handle create_vertex_buffer(const float* vertices, size_t sizev, const attribute* attributes, size_t sizea) override;
        void submit(const vertex_buffer_handle& vertex_buffer, const program_handle& program) override;
    };
}
