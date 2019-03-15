#pragma once

#include <glm/glm.hpp>
#include <graphics/texture.hpp>
#include <string>
#include <variant>

namespace moka
{
    using parameter =
        std::variant<float, glm::vec3, glm::vec4, glm::mat3, glm::mat4, texture>;

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

        material_parameter(const std::string& name, const texture& data, size_t count = 1);

        material_parameter& operator=(float data);

        material_parameter& operator=(const glm::vec3& data);

        material_parameter& operator=(const glm::vec4& data);

        material_parameter& operator=(const glm::mat3& data);

        material_parameter& operator=(const glm::mat4& data);

        material_parameter& operator=(const texture& data);

        material_parameter& operator=(glm::vec3&& data);

        material_parameter& operator=(glm::vec4&& data);

        material_parameter& operator=(glm::mat3&& data);

        material_parameter& operator=(glm::mat4&& data);

        material_parameter& operator=(texture&& data);

        material_parameter& operator=(const material_parameter& rhs);

        material_parameter& operator=(material_parameter&& rhs) noexcept;

        material_parameter(const material_parameter& rhs) = default;

        material_parameter(material_parameter&& rhs) noexcept = default;
    };
} // namespace moka
