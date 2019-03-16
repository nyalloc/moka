#pragma once

#include <graphics/command/graphics_command.hpp>
#include <graphics/material/material.hpp>

namespace moka
{
    class set_material_parameters_command final : public graphics_command
    {
    public:
        material_handle material;

        std::vector<material_parameter> parameters;

        virtual ~set_material_parameters_command();

        set_material_parameters_command& set_material(material_handle material);

        set_material_parameters_command& set_parameter(const std::string& name, float data);

        set_material_parameters_command& set_parameter(const std::string& name, const glm::vec3& data);

        set_material_parameters_command& set_parameter(const std::string& name, const glm::vec4& data);

        set_material_parameters_command& set_parameter(const std::string& name, const glm::mat3& data);

        set_material_parameters_command& set_parameter(const std::string& name, const glm::mat4& data);

        set_material_parameters_command& set_parameter(const std::string& name, texture_handle data);

        void accept(graphics_visitor& visitor) override;
    };
} // namespace moka
