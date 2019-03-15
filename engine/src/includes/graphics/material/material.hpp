#pragma once

#include <graphics/api/graphics_api.hpp>
#include <graphics/material/material_parameter.hpp>
#include <graphics/material/parameter_collection.hpp>
#include <vector>

namespace moka
{
    using material_handle = uint16_t;

    enum class polygon_draw_mode
    {
        fill,
        line,
        points
    };

    struct polygon_mode final
    {
        polygon_draw_mode mode = polygon_draw_mode::fill;
        face faces = face::front_and_back;
    };

    struct culling final
    {
        bool enabled = true;
        face faces = face::back;
    };

    struct blend final
    {
        bool enabled = false;
        blend_equation equation = blend_equation::func_add;
        blend_function_factor source = blend_function_factor::src_alpha;
        blend_function_factor destination = blend_function_factor::one_minus_src_alpha;
    };

    class material_builder;

    class material final
    {
    protected:
        alpha_mode alpha_mode_ = alpha_mode::opaque;
        program_handle program_ = {std::numeric_limits<uint16_t>::max()};
        parameter_collection parameters_;
        blend blend_;
        culling culling_;
        polygon_mode polygon_mode_;
        bool depth_test_ = true;
        bool scissor_test_ = false;

    public:
        using iterator = parameter_collection::iterator;
        using const_iterator = parameter_collection::const_iterator;

        using builder = material_builder;

        material();

        ~material();

        material(const material& rhs);

        material& operator=(const material& rhs);

        material(material&& rhs) noexcept;

        material& operator=(material&& rhs) noexcept;

        material(
            program_handle program_handle,
            parameter_collection&& parameters,
            alpha_mode alpha_mode,
            const blend& blend,
            const culling& culling,
            const polygon_mode& polygon_mode,
            bool depth_test,
            bool scissor_test);

        explicit material(program_handle program_handle);

        alpha_mode get_alpha_mode() const;

        program_handle get_program() const;

        size_t size() const;

        bool get_scissor_test() const;

        bool get_depth_test() const;

        const_iterator begin() const;

        const_iterator end() const;

        iterator begin();

        iterator end();

        const material_parameter& operator[](size_t index) const;

        material_parameter& operator[](size_t index);

        material_parameter& operator[](const std::string& name);

        const blend& get_blend() const;

        const culling& get_culling() const;

        const polygon_mode& get_polygon_mode() const;
    };
} // namespace moka
