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

#include <graphics/command/graphics_command.hpp>
#include <graphics/material/material.hpp>

namespace moka
{
    /**
     * \brief Update material parameters before drawing.
     */
    class set_material_parameters_command final : public graphics_command
    {
    public:
        material_handle material; /**< The material to update. */

        std::vector<material_parameter> parameters; /**< The material parameters to update. */

        virtual ~set_material_parameters_command();

        /**
         * \brief Set the material that you want to update.
         * \param material The material that you want to update.
         * \return A reference to this set_material_parameters_command object to enable method chaining.
         */
        set_material_parameters_command& set_material(material_handle material);

        /**
         * \brief Update a material parameter.
         * \param name The name of the material parameter you want to update.
         * \param data The new data you wish to update the material parameter with.
         * \return A reference to this set_material_parameters_command object to enable method chaining.
         */
        set_material_parameters_command& set_parameter(const std::string& name, float data);

        /**
         * \brief Update a material parameter.
         * \param name The name of the material parameter you want to update.
         * \param data The new data you wish to update the material parameter with.
         * \return A reference to this set_material_parameters_command object to enable method chaining.
         */
        set_material_parameters_command& set_parameter(const std::string& name, const glm::vec3& data);

        /**
         * \brief Update a material parameter.
         * \param name The name of the material parameter you want to update.
         * \param data The new data you wish to update the material parameter with.
         * \return A reference to this set_material_parameters_command object to enable method chaining.
         */
        set_material_parameters_command& set_parameter(const std::string& name, const glm::vec4& data);

        /**
         * \brief Update a material parameter.
         * \param name The name of the material parameter you want to update.
         * \param data The new data you wish to update the material parameter with.
         * \return A reference to this set_material_parameters_command object to enable method chaining.
         */
        set_material_parameters_command& set_parameter(const std::string& name, const glm::mat3& data);

        /**
         * \brief Update a material parameter.
         * \param name The name of the material parameter you want to update.
         * \param data The new data you wish to update the material parameter with.
         * \return A reference to this set_material_parameters_command object to enable method chaining.
         */
        set_material_parameters_command& set_parameter(const std::string& name, const glm::mat4& data);

        /**
         * \brief Update a material parameter.
         * \param name The name of the material parameter you want to update.
         * \param data The new data you wish to update the material parameter with.
         * \return A reference to this set_material_parameters_command object to enable method chaining.
         */
        set_material_parameters_command& set_parameter(const std::string& name, texture_handle data);

        /**
         * \brief Accept a graphics_visitor object. Invoke this command using
         * the graphics_visitor. \param visitor A graphics_visitor object.
         */
        void accept(graphics_visitor& visitor) override;
    };
} // namespace moka
