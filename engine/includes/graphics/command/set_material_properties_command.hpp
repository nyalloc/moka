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
         * \brief Accept a graphics_visitor object. Invoke this command using the graphics_visitor.
         * \param visitor A graphics_visitor object.
         */
        void accept(graphics_visitor& visitor) override;
    };
} // namespace moka
