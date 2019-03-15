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
        const std::string& name, texture data)
    {
        parameters.emplace_back(material_parameter{name, data});
        return *this;
    }

    void set_material_parameters_command::accept(graphics_visitor& visitor)
    {
        visitor.visit(*this);
    }
} // namespace moka
