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

#include <graphics/api/graphics_api.hpp>
#include <graphics/material/material_parameter.hpp>
#include <graphics/material/parameter_collection.hpp>
#include <vector>

namespace moka
{
    /**
     * \brief A unique identifier for a material.
     */

    struct material_handle
    {
        uint16_t id;
    };

    enum class polygon_draw_mode
    {
        fill,
        line,
        points
    };

    /**
     * \brief The polygon mode to use with a material.
     */
    struct polygon_mode final
    {
        polygon_draw_mode mode = polygon_draw_mode::fill;
        face faces = face::front_and_back;
    };

    /**
     * \brief Culling settings to use with a material.
     */
    struct culling final
    {
        bool enabled = true;
        face faces = face::back;
    };

    /**
     * \brief Blend settings to use with a material.
     */
    struct blend final
    {
        bool enabled = false;
        blend_equation equation = blend_equation::func_add;
        blend_function_factor source = blend_function_factor::src_alpha;
        blend_function_factor destination = blend_function_factor::one_minus_src_alpha;
    };

    class material_builder;

    /**
     * \brief A material that can be used when rendering a primitive.
     */
    class material final
    {
        alpha_mode alpha_mode_ = alpha_mode::opaque;
        std::vector<program_handle> programs_ = {{std::numeric_limits<uint16_t>::max()}};
        size_t active_program_ = 0;
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

        /**
         * \brief Create a new material object.
         * \param program_handles The programs that can be used to render this
         * material. \param parameters The parameters to use with the material.
         * \param alpha_mode The alpha mode to use with the material.
         * \param blend The blend mode to use with the material.
         * \param culling The culling mode to use with the material.
         * \param polygon_mode The polygon mode to use with the material.
         * \param depth_test Does this material require the depth test?
         * \param scissor_test Does this material require the scissor test?
         */
        material(
            std::vector<program_handle>&& program_handles,
            parameter_collection&& parameters,
            alpha_mode alpha_mode,
            const blend& blend,
            const culling& culling,
            const polygon_mode& polygon_mode,
            bool depth_test,
            bool scissor_test);

        /**
         * \brief Create a new material object.
         * \param program_handle The program to use with this material.
         */
        explicit material(program_handle program_handle);

        /**
         * \brief Get the alpha mode of this material.
         * \return This material's alpha mode.
         */
        alpha_mode get_alpha_mode() const;

        /**
         * \brief Get this material's program.
         * \return This material's program.
         */
        program_handle get_program() const;

        /**
         * \brief Get the number of parameters in this material.
         * \return The number of parameters in this material.
         */
        size_t size() const;

        /**
         * \brief Does this material require the scissor test?
         * \return True if scissor test is required, otherwise false.
         */
        bool get_scissor_test() const;

        /**
         * \brief Does this material require the depth test?
         * \return True if depth test is required, otherwise false.
         */
        bool get_depth_test() const;

        const_iterator begin() const;

        const_iterator end() const;

        iterator begin();

        iterator end();

        /**
         * \brief Return the material parameter at the index using the subscript operator.
         * \param index The index of the material parameter.
         * \return The material parameter.
         */
        const material_parameter& operator[](size_t index) const;

        /**
         * \brief Return the material parameter at the index using the subscript operator.
         * \param index The index of the material parameter.
         * \return The material parameter.
         */
        material_parameter& operator[](size_t index);

        /**
         * \brief Return the material parameter with the name using the
         * subscript operator. \param name The name of the material parameter.
         * \return The material parameter.
         */
        material_parameter& operator[](const std::string& name);

        /**
         * \brief Get the blend mode of this material.
         * \return The blend mode of this material.
         */
        const blend& get_blend() const;

        /**
         * \brief Get the culling mode of this material.
         * \return The culling mode of this material.
         */
        const culling& get_culling() const;

        /**
         * \brief Get the polygon mode of this material.
         * \return The polygon mode of this material.
         */
        const polygon_mode& get_polygon_mode() const;

        /**
         * \brief Set this material's active program.
         * \param index The index of the program you want to use to render this material.
         */
        void set_active_program(size_t active_program);
    };
} // namespace moka
