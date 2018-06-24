#pragma once

#include <ostream>
#include <maths/colour.hpp>
#include <maths/vector2.hpp>
#include <maths/vector3.hpp>

namespace neon
{
    struct rectangle
    {
        int x{}, y{}, width{}, height{};
    };

    struct vertex
    {
        vector3 position{ 0.0f, 0.0f, 0.0f };
        vector2 texture_coordinates{ 0.0f, 0.0f };
    };

    enum class primitive_type
    {
        lines,
        line_strip,
        triangles,
        triangle_strip,
    };

    enum class toggle
    {
        enable,
        disable
    };

    enum class cull_face
    {
        front,
        back,
        front_and_back
    };

    enum class blend_function_factor
    {
        zero,
        one,
        src_colour,
        one_minus_src_colour,
        dst_colour,
        one_minus_dst_colour,
        src_alpha,
        one_minus_src_alpha,
        dst_alpha,
        one_minus_dst_alpha,
        constant_colour,
        one_minus_constant_colour,
        constant_alpha,
        one_minus_constant_alpha
    };

    enum class shader_attribute
    {
        position,   //!< a_position
        normal,     //!< a_normal
        tangent,    //!< a_tangent
        bitangent,  //!< a_bitangent
        color0,     //!< a_color0
        color1,     //!< a_color1
        color2,     //!< a_color2
        color3,     //!< a_color3
        indices,    //!< a_indices
        weight,     //!< a_weight
        tex_coord0, //!< a_texcoord0
        tex_coord1, //!< a_texcoord1
        tex_coord2, //!< a_texcoord2
        tex_coord3, //!< a_texcoord3
        tex_coord4, //!< a_texcoord4
        tex_coord5, //!< a_texcoord5
        tex_coord6, //!< a_texcoord6
        tex_coord7, //!< a_texcoord7
    };

    enum class uniform_type
    {
        int_uniform,   //!< integer uniform
        float_uniform, //!< float uniform
        vec3_uniform,  //!< 3 floats vector uniform
        vec4_uniform,  //!< 4 floats vector uniform
        mat3_uniform,  //!< 3x3 matrix uniform
        mat4_uniform,  //!< 4x4 matrix uniform
    };

    using handle = uint16_t;
    
    struct dynamic_index_buffer_handle
    {
        handle id;
    };

    struct dynamic_vertex_buffer_handle 
    {
        handle id;
    };

    struct frame_buffer_handle 
    {
        handle id;
    };

    struct index_buffer_handle 
    {
        handle id;
    };

    struct indirect_buffer_handle 
    {
        handle id;
    };

    struct occlusion_query_handle 
    {
        handle id;
    };

    struct program_handle 
    {
        handle id;
    };

    struct shader_handle 
    {
        handle id;
    };

    struct texture_handle 
    {
        handle id;
    };

    struct uniform_handle 
    {
        handle id;
    };

    struct vertex_buffer_handle 
    {
        handle id;
    };

    struct vertex_decl_handle 
    {
        handle id;
    };

    /**
     * \brief render_context abstracts the native rendering API.
     */
    class graphics_api_impl
    {
    public:
        virtual ~graphics_api_impl() = default;
        virtual std::string version() const = 0;
        virtual std::string vendor() const = 0;
        virtual std::string renderer() const = 0;
        virtual std::string extensions() const = 0;
        virtual std::string shading_language_version() const = 0;
        virtual void clear(bool color, bool depth, bool stencil, byte stencilValue, const colour& colour) const = 0;
        virtual void depth_bounds_test(float zmin, float zmax) const = 0;
        virtual void polygon_offset(float scale, float bias) const = 0;
        virtual void viewport(const rectangle& rectangle) const = 0;
        virtual void scissor(const rectangle& rectangle) const = 0;
        virtual void check_errors() const = 0;
        virtual void clear_colour(const colour& colour) const = 0;
        virtual void set_scissor(const rectangle& bounds) const = 0;
        virtual void set_viewport(const rectangle& bounds) const = 0;
        virtual void draw_indexed_primitives(primitive_type primitive_type, size_t index_count) const = 0;
        virtual void draw_primatives(primitive_type primitive_type, size_t vertex_count) const = 0;
        virtual void scissor_test(toggle toggle) const = 0;
        virtual void depth_mask(toggle toggle) const = 0;
        virtual void depth_test(toggle toggle) const = 0;
        virtual void face_culling(toggle toggle) const = 0;
        virtual void blend(toggle toggle) const = 0;
        virtual void blend_function(blend_function_factor lhs, blend_function_factor rhs) const = 0;
        virtual program_handle create_program(const shader_handle& vertex_handle, const shader_handle& fragment_handle) = 0;
        virtual void destroy(const shader_handle& handle) = 0;
    };
}