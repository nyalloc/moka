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
#include <graphics/material/material_parameter.hpp>

namespace moka
{
    material_parameter::material_parameter() = default;

    material_parameter::~material_parameter() = default;

    material_parameter::material_parameter(
        const std::string& name, const parameter_type type, const parameter& data, const size_t count)
        : type(type), name(name), data(data), count(count)
    {
    }

    material_parameter::material_parameter(const std::string& name, const float data, const size_t count)
        : material_parameter(name, parameter_type::float32, data, count)
    {
    }

    material_parameter::material_parameter(const std::string& name, const glm::vec3& data, const size_t count)
        : material_parameter(name, parameter_type::vec3, data, count)
    {
    }

    material_parameter::material_parameter(const std::string& name, const glm::vec4& data, const size_t count)
        : material_parameter(name, parameter_type::vec4, data, count)
    {
    }

    material_parameter::material_parameter(const std::string& name, const glm::mat3& data, const size_t count)
        : material_parameter(name, parameter_type::mat3, data, count)
    {
    }

    material_parameter::material_parameter(const std::string& name, const glm::mat4& data, const size_t count)
        : material_parameter(name, parameter_type::mat4, data, count)
    {
    }

    material_parameter& material_parameter::operator=(const float data)
    {
        this->data = data;
        this->count = 1;
        type = parameter_type::float32;
        return *this;
    }

    material_parameter& material_parameter::operator=(const glm::vec3& data)
    {
        this->data = data;
        this->count = 1;
        type = parameter_type::vec3;
        return *this;
    }

    material_parameter& material_parameter::operator=(const glm::vec4& data)
    {
        this->data = data;
        this->count = 1;
        type = parameter_type::vec4;
        return *this;
    }

    material_parameter& material_parameter::operator=(const glm::mat3& data)
    {
        this->data = data;
        this->count = 1;
        type = parameter_type::mat3;
        return *this;
    }

    material_parameter& material_parameter::operator=(const glm::mat4& data)
    {
        this->data = data;
        this->count = 1;
        type = parameter_type::mat4;
        return *this;
    }

    material_parameter& material_parameter::operator=(const texture_handle& data)
    {
        this->data = data;
        this->count = 1;
        type = parameter_type::texture;
        return *this;
    }

    material_parameter& material_parameter::operator=(glm::vec3&& data)
    {
        this->data = data;
        this->count = 1;
        type = parameter_type::vec3;
        return *this;
    }

    material_parameter& material_parameter::operator=(glm::vec4&& data)
    {
        this->data = data;
        this->count = 1;
        type = parameter_type::vec4;
        return *this;
    }

    material_parameter& material_parameter::operator=(glm::mat3&& data)
    {
        this->data = data;
        this->count = 1;
        type = parameter_type::mat3;
        return *this;
    }

    material_parameter& material_parameter::operator=(glm::mat4&& data)
    {
        this->data = data;
        this->count = 1;
        type = parameter_type::mat4;
        return *this;
    }

    material_parameter& material_parameter::operator=(texture_handle&& data)
    {
        this->data = data;
        this->count = 1;
        type = parameter_type::texture;
        return *this;
    }

    material_parameter& material_parameter::operator=(const material_parameter& rhs) = default;

    material_parameter& material_parameter::operator=(material_parameter&& rhs) noexcept = default;

    material_parameter::material_parameter(
        const std::string& name, const texture_handle& data, const size_t count)
        : material_parameter(name, parameter_type::texture, data, count)
    {
    }
} // namespace moka
