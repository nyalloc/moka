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

#include <graphics/material/material.hpp>
#include <graphics/material/material_properties.hpp>
#include <graphics/texture_handle.hpp>
#include <string>
#include <vector>

namespace moka
{
    class graphics_device;

    /**
     * \brief A builder class to help you build a material.
     */
    class material_builder
    {
        graphics_device& graphics_device_;

        std::vector<material_property> texture_maps_;

        parameter_collection parameters_;
        std::vector<std::string> fragment_shaders_src_;
        std::vector<std::string> vertex_shaders_src_;
        size_t active_program_ = 0;

        alpha_mode alpha_mode_ = alpha_mode::opaque;
        blend blend_;
        culling culling_;
        polygon_mode polygon_mode_;
        bool scissor_test_ = false;
        bool depth_test_ = true;

        static std::string get_property_name(material_property property);

        static bool replace(std::string& source, const std::string& target, const std::string& replacement);

        void build_shader_source();

    public:
        /**
         * \brief Create a material builder.
         * \param device Graphics device class to use when building this material.
         */
        explicit material_builder(graphics_device& device);

        /**
         * \brief Set the vertex shader for use in this material.
         * \param vertex_shader The path to the vertex shader.
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& add_vertex_shader(const std::filesystem::path& vertex_shader);

        /**
         * \brief Set the vertex shader for use in this material.
         * \param vertex_shader The source code of the shader.
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& add_vertex_shader(const char* vertex_shader);

        /**
         * \brief Set the vertex shader for use in this material.
         * \param vertex_shader The source code of the shader.
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& add_vertex_shader(const std::string& vertex_shader);

        /**
         * \brief Set the fragment shader for use in this material.
         * \param fragment_shader The path to the fragment shader.
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& add_fragment_shader(const std::filesystem::path& fragment_shader);

        /**
         * \brief Set the fragment shader for use in this material.
         * \param fragment_shader The source code of the shader.
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& add_fragment_shader(const char* fragment_shader);

        /**
         * \brief Set the fragment shader for use in this material.
         * \param fragment_shader The source code of the shader.
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& add_fragment_shader(const std::string& fragment_shader);

        /**
         * \brief Set the blend equation for use in this material.
         * \param equation The equation for use in this material.
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& set_blend_equation(blend_equation equation);

        /**
         * \brief Set the blend function for use in this material.
         * \param source The source blend function factor.
         * \param destination The destination blend function factor.
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& set_blend_function(blend_function_factor source, blend_function_factor destination);

        /**
         * \brief Specify if this material should use blending.
         * \param enabled True if the material should use blending, otherwise false.
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& set_blend_enabled(bool enabled);

        /**
         * \brief Specify if this material should use culling.
         * \param enabled True if the material should use culling, otherwise false.
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& set_culling_enabled(bool enabled);

        /**
         * \brief Specify if this material should use depth test.
         * \param enabled True if the material should use depth test, otherwise false.
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& set_depth_test_enabled(bool enabled);

        /**
         * \brief Specify if this material should use scissor test.
         * \param enabled True if the material should use the scissor test, otherwise false.
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& set_scissor_test_enabled(bool enabled);

        /**
         * \brief Specify which faces should be culled if culling is enabled.
         * \param faces The faces that should be culled if culling is enabled.
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& set_culling_faces(face faces);

        /**
         * \brief Specify what alpha mode should be used.
         * \param alpha_mode The alpha mode to use with this material.
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& set_alpha_mode(alpha_mode alpha_mode);

        /**
         * \brief Set the polygon mode of this material.
         * \param faces Specifies the polygons that the mode applies to.
         * \param mode Specifies how the polygons will be rasterized.
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& set_polygon_mode(face faces, polygon_draw_mode mode);

        /**
         * \brief Add a material parameter to this material.
         * \param name The name of the material.
         * \param type The type of the material (set with no initial value).
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& add_material_parameter(const std::string& name, parameter_type type);

        /**
         * \brief Add a material parameter to this material.
         * \param name The name of the material.
         * \param data The initial state of this material parameter.
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& add_material_parameter(const std::string& name, float data);

        /**
         * \brief Add a material parameter to this material.
         * \param name The name of the material.
         * \param data The initial state of this material parameter.
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& add_material_parameter(const std::string& name, const texture_handle& data);

        /**
         * \brief Add a material parameter to this material.
         * \param name The name of the material.
         * \param data The initial state of this material parameter.
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& add_material_parameter(const std::string& name, const glm::vec3& data);

        /**
         * \brief Add a material parameter to this material.
         * \param name The name of the material.
         * \param data The initial state of this material parameter.
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& add_material_parameter(const std::string& name, const glm::vec4& data);

        /**
         * \brief Add a material parameter to this material.
         * \param name The name of the material.
         * \param data The initial state of this material parameter.
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& add_material_parameter(const std::string& name, const glm::mat3& data);

        /**
         * \brief Add a material parameter to this material.
         * \param name The name of the material.
         * \param data The initial state of this material parameter.
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& add_material_parameter(const std::string& name, const glm::mat4& data);

        /**
         * \brief Add a texture as a material parameter to this material.
         * \param property The material property that the texture will bound to.
         * \param texture The texture that should be used.
         * \return A reference to this material_builder object to enable method chaining.
         */
        material_builder& add_texture(material_property property, texture_handle texture);

        /**
         * \brief Build the final material.
         * \return The new material handle.
         */
        material_handle build();
    };
} // namespace moka
