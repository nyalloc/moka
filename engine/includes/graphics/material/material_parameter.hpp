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

#include <glm/glm.hpp>
#include <graphics/texture_handle.hpp>
#include <string>
#include <variant>

namespace moka
{
    using parameter =
        std::variant<float, glm::vec3, glm::vec4, glm::mat3, glm::mat4, texture_handle>;

    enum class parameter_type : uint8_t
    {
        texture, //!< sampler
        vec3,    //!< 3 floats vector uniform
        vec4,    //!< 4 floats vector uniform
        mat3,    //!< 3x3 matrix uniform
        mat4,    //!< 4x4 matrix uniform
        float32, //! single floating point uniform
        null     //!< value not yet assigned to
    };

    struct material_parameter final
    {
        parameter_type type = parameter_type::null;
        std::string name;
        parameter data;
        size_t count = 1;

        material_parameter();

        ~material_parameter();

        material_parameter(
            const std::string& name, parameter_type type, const parameter& data = {}, size_t count = 1);

        material_parameter(const std::string& name, float data, size_t count = 1);

        material_parameter(const std::string& name, const glm::vec3& data, size_t count = 1);

        material_parameter(const std::string& name, const glm::vec4& data, size_t count = 1);

        material_parameter(const std::string& name, const glm::mat3& data, size_t count = 1);

        material_parameter(const std::string& name, const glm::mat4& data, size_t count = 1);

        material_parameter(const std::string& name, const texture_handle& data, size_t count = 1);

        material_parameter& operator=(float data);

        material_parameter& operator=(const glm::vec3& data);

        material_parameter& operator=(const glm::vec4& data);

        material_parameter& operator=(const glm::mat3& data);

        material_parameter& operator=(const glm::mat4& data);

        material_parameter& operator=(const texture_handle& data);

        material_parameter& operator=(glm::vec3&& data);

        material_parameter& operator=(glm::vec4&& data);

        material_parameter& operator=(glm::mat3&& data);

        material_parameter& operator=(glm::mat4&& data);

        material_parameter& operator=(texture_handle&& data);

        material_parameter& operator=(const material_parameter& rhs);

        material_parameter& operator=(material_parameter&& rhs) noexcept;

        material_parameter(const material_parameter& rhs) = default;

        material_parameter(material_parameter&& rhs) noexcept = default;
    };
} // namespace moka
