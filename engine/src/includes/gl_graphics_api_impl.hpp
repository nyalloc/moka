#pragma once

#include <graphics_api_impl.hpp>
#include <GL/glew.h>
#include <unordered_map>

namespace neon
{
    /**
     * \brief Convert the interface of OpenGL into the neon rendring API. 
     * Allows neon rendering functionality to work at a higher level without being coupled to an OpenGL backend.
     */
    class gl_graphics_api_impl : public graphics_api_impl
    {
        std::unordered_map<handle, GLuint> shaders_;
        std::unordered_map<handle, GLuint> programs_;
    public:
        std::string version() const override;
        std::string vendor() const override;
        std::string renderer() const override;
        std::string extensions() const override;
        std::string shading_language_version() const override;
        void clear(bool color, bool depth, bool stencil, byte stencil_value, const colour& colour) const override;
        void depth_bounds_test(float zmin, float zmax) const override;
        void polygon_offset(float scale, float bias) const override;
        void viewport(const rectangle& rectangle) const override;
        void scissor(const rectangle& rectangle) const override;
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
    };
}
