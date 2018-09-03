#pragma once

#include <graphics/graphics_api.hpp>

namespace moka
{
    /**
     * \brief Convert the interface of OpenGL into the moka rendring API.
     * Allows moka rendering functionality to work at a higher level without being coupled to an OpenGL backend.
     */
    class gl_graphics_api : public graphics_api
    {
        logger log_;
        std::vector<shader_handle> shaders_;
        std::vector<program_handle> programs_;
        std::vector<vertex_buffer_handle> vertex_buffers_;
    public:
        gl_graphics_api();
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
        vertex_buffer_handle create_vertex_buffer(const memory& vertices, const vertex_decl& decl) override;

		void draw_indexed(const primitive_type type, size_t first, size_t count) const override;
		void bind(const vertex_buffer_handle& program) const override;
		void bind(const program_handle& program) const override;
	};
}
