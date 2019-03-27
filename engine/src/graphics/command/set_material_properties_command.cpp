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
#include <graphics/command/set_material_properties_command.hpp>

namespace moka
{
    set_material_parameters_command::~set_material_parameters_command() = default;

    set_material_parameters_command& set_material_parameters_command::set_material(material_handle material)
    {
        this->material = material;
        return *this;
    }

    set_material_parameters_command& set_material_parameters_command::set_parameter(
        const std::string& name, float data)
    {
        parameters.emplace_back(material_parameter{name, data});
        return *this;
    }

    set_material_parameters_command& set_material_parameters_command::set_parameter(
        const std::string& name, const glm::vec3& data)
    {
        parameters.emplace_back(material_parameter{name, data});
        return *this;
    }

    set_material_parameters_command& set_material_parameters_command::set_parameter(
        const std::string& name, const glm::vec4& data)
    {
        parameters.emplace_back(material_parameter{name, data});
        return *this;
    }

    set_material_parameters_command& set_material_parameters_command::set_parameter(
        const std::string& name, const glm::mat3& data)
    {
        parameters.emplace_back(material_parameter{name, data});
        return *this;
    }

    set_material_parameters_command& set_material_parameters_command::set_parameter(
        const std::string& name, const glm::mat4& data)
    {
        parameters.emplace_back(material_parameter{name, data});
        return *this;
    }

    set_material_parameters_command& set_material_parameters_command::set_parameter(
        const std::string& name, texture_handle data)
    {
        parameters.emplace_back(material_parameter{name, data});
        return *this;
    }

    void set_material_parameters_command::accept(graphics_visitor& visitor)
    {
        visitor.visit(*this);
    }
} // namespace moka
