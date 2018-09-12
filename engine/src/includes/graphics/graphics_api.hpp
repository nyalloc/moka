#pragma once

#include <ostream>
#include <maths/colour.hpp>
#include <maths/vector3.hpp>
#include <vector>

namespace moka
{
    struct memory
    {
        const uint8_t* data;
        size_t size;
    };

    inline memory make_buffer(const void* buffer, const size_t size)
    {
        memory result{};
        result.size = size;
        result.data = static_cast<const uint8_t*>(buffer);
        return result;
    }

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

    enum class attribute_type
    {
        int8,   
        int16,
        int32,
        int64,
        uint8,
        uint16,
        uint32,
        uint64,
        float16,
        float32,
        float64,
        boolean,
    };

    inline size_t attr_type_size(const attribute_type attr_type)
    {
        switch (attr_type)
        {
        case attribute_type::int8:
            return sizeof(int8_t);
        case attribute_type::int16:
            return sizeof(int16_t);
        case attribute_type::int32:
            return sizeof(int32_t);
        case attribute_type::int64:
            return sizeof(int64_t);
        case attribute_type::uint8:
            return sizeof(uint8_t);
        case attribute_type::uint16:
            return sizeof(uint16_t);
        case attribute_type::uint32:
            return sizeof(uint32_t);
        case attribute_type::uint64:
            return sizeof(int64_t);
        case attribute_type::float32:
            return sizeof(float);
        case attribute_type::float64:
            return sizeof(double);
        default:
            return 0;
        }
    }

    struct vertex_layout;

    struct attribute_element
    {
        attribute attr;
        size_t size;
        attribute_type type;
        bool normalized;

        constexpr attribute_element(
            const attribute attr,
            const size_t element_count,
            const attribute_type attr_type,
            const bool normalised) noexcept
            : attr(attr),
            size(element_count),
            type(attr_type),
            normalized(normalised)
        {}
    };

    
    struct vertex_layout_builder;

    struct vertex_layout
    {
    private:
        std::vector<attribute_element> attributes_;
    public:
        void emplace(attribute attr, size_t size, attribute_type attr_type, bool normalised)
        {
            attributes_.emplace_back(attr, size, attr_type, normalised);
        }

        static vertex_layout_builder builder() noexcept;

        size_t size() const noexcept;

        size_t stride() const noexcept;

        auto begin() noexcept
        {
            return attributes_.begin();
        }

        auto end() noexcept
        {
            return attributes_.end();
        }

        auto begin() const noexcept
        {
            return attributes_.begin();
        }

        auto end() const noexcept
        {
            return attributes_.end();
        }
    };

    inline size_t vertex_layout::stride() const noexcept
    {
        size_t stride = 0;
        for (const auto& element : attributes_)
        {
            stride += attr_type_size(element.type) * element.size;
        }
        return stride;
    }

    struct vertex_layout_builder
    {
    private:
        vertex_layout attr;
    public:
        vertex_layout_builder& add_attribute(attribute attr, size_t size, attribute_type attr_type, bool normalised);
        vertex_layout build() const;
    };

    inline vertex_layout_builder vertex_layout::builder() noexcept
    {
        return {};
    }

    inline size_t vertex_layout::size() const noexcept
    {
        size_t result = 0;
        for (const auto& element : attributes_)
        {
            result += element.size;
        }
        return result;
    }

    inline vertex_layout_builder& vertex_layout_builder::add_attribute(
        const attribute attr, 
        const size_t size,
        const attribute_type attr_type,
        const bool normalised = false)
    {
        this->attr.emplace(attr, size, attr_type, normalised);
        return *this;
    }

    inline vertex_layout vertex_layout_builder::build() const
    {
        return attr;
    }

    template<typename T>
    class basic_rectangle
    {
        T x_, y_, width_, height_;
    public:
        constexpr basic_rectangle(const T x, const T y, const T width, const T height) noexcept
            : x_{ x }, y_{ y }, width_{ width }, height_{ height }
        {}

        constexpr T x() const noexcept
        {
            return x_;
        }

        constexpr T y() const noexcept
        {
            return y_;
        }

        constexpr T width() const noexcept
        {
            return width_;
        }

        constexpr T height() const noexcept
        {
            return height_;
        }
    };

    using rectangle = basic_rectangle<int>;

    enum class primitive_type : uint8_t
    {
		triangles,
		triangle_strip,
        lines,
        line_strip,
		points,
		zero
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

    enum class shader_attribute : uint8_t
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
    };

    enum class uniform_type : uint8_t
    {
        int1, //!< integer uniform
        vec3, //!< 3 floats vector uniform
        vec4, //!< 4 floats vector uniform
        mat3, //!< 3x3 matrix uniform
        mat4, //!< 4x4 matrix uniform
    };

    enum class shader_type : uint8_t
    {
        vertex,	  
        fragment, 
        compute   
    };

	enum class face_culling : uint8_t
	{
		front,
		back,
		front_and_back
	};

    using handle_id = uint16_t;

    struct dynamic_index_buffer_handle
    {
		handle_id id = std::numeric_limits<moka::handle_id>::max();
    };

    struct dynamic_vertex_buffer_handle
    {
		handle_id id = std::numeric_limits<moka::handle_id>::max();
    };

    struct frame_buffer_handle
    {
		handle_id id = std::numeric_limits<moka::handle_id>::max();
    };

    struct index_buffer_handle
    {
		handle_id id = std::numeric_limits<moka::handle_id>::max();
    };

    struct indirect_buffer_handle
    {
		handle_id id = std::numeric_limits<moka::handle_id>::max();
    };

    struct occlusion_query_handle
    {
		handle_id id = std::numeric_limits<moka::handle_id>::max();
    };

    struct program_handle
    {
		handle_id id = std::numeric_limits<moka::handle_id>::max();
    };

    struct shader_handle
    {
		handle_id id = std::numeric_limits<moka::handle_id>::max();
    };

    struct texture_handle
    {
		handle_id id = std::numeric_limits<moka::handle_id>::max();
    };

	struct uniform_handle
	{
		handle_id id = std::numeric_limits<moka::handle_id>::max();
	};

    struct uniform_buffer_handle
    {
		handle_id id = std::numeric_limits<moka::handle_id>::max();
    };

    struct vertex_buffer_handle
    {
		handle_id id = std::numeric_limits<moka::handle_id>::max();
    };

    struct vertex_decl_handle
    {
		handle_id id = std::numeric_limits<moka::handle_id>::max();
    };

	template<typename T>
	bool is_handle_valid(const T& handle)
	{
		return handle.id != std::numeric_limits<moka::handle_id>::max();
	}

    /**
     * \brief render_context abstracts the native rendering API.
     */
    class graphics_api
    {
    public:
        virtual ~graphics_api() = default;

        virtual std::string version() const = 0;
        virtual std::string vendor() const = 0;
        virtual std::string renderer() const = 0;
        virtual std::string extensions() const = 0;
        virtual std::string shading_language_version() const = 0;

        virtual void clear(bool color, bool depth, bool stencil, byte stencil_value, const colour& colour) const = 0;
        virtual void depth_bounds_test(float zmin, float zmax) const = 0;
        virtual void polygon_offset(float scale, float bias) const = 0;
        virtual void check_errors() const = 0;
        virtual void clear_colour(const colour& colour) const = 0;
        virtual void set_scissor(const rectangle& bounds) const = 0;
        virtual void set_viewport(const rectangle& bounds) const = 0;
        virtual void scissor_test(toggle toggle) const = 0;
        virtual void depth_mask(toggle toggle) const = 0;
        virtual void depth_test(toggle toggle) const = 0;
        virtual void face_culling(toggle toggle) const = 0;
        virtual void blend_function(blend_function_factor lhs, blend_function_factor rhs) const = 0;
        virtual void destroy(const shader_handle& handle) = 0;
		virtual void blend(toggle toggle) const = 0;

		virtual program_handle create_program(const shader_handle& vertex_handle, const shader_handle& fragment_handle) = 0;
        virtual shader_handle create_shader(shader_type type, const std::string& source) = 0;
        virtual vertex_buffer_handle create_vertex_buffer(const memory& vertices, const vertex_layout& decl) = 0;

		virtual void draw_arrays(primitive_type type, size_t first, size_t count) const = 0;
		virtual void draw_indexed(primitive_type type, size_t count) const = 0;

		virtual void bind(const vertex_buffer_handle& program) const = 0;
		virtual void bind(const program_handle& program) const = 0;
    };
}
