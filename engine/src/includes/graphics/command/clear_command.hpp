#pragma once

#include <glm/detail/type_vec4.hpp>
#include <graphics/command/graphics_command.hpp>

namespace moka
{
    /**
     * \brief Clear the current frame buffer.
     */
    class clear_command final : public graphics_command
    {
    public:
        glm::vec4 color;          /**< The color that should be used to clear the buffer. */
        bool clear_depth = false; /**< Should we clear the depth information? */
        bool clear_color = false; /**< Should we clear the color buffer? */

        virtual ~clear_command();

        void accept(graphics_visitor& visitor) override;

        /**
         * \brief Set the color of the clear_command.
         * \param color The color that should be used.
         * \return A reference to this clear_command object to enable method chaining.
         */
        clear_command& set_color(const glm::vec4& color);

        /**
         * \brief Set the color of the clear_command.
         * \param r The r component of the color.
         * \param g The g component of the color.
         * \param b The b component of the color.
         * \param a The a component of the color.
         * \return A reference to this clear_command object to enable method chaining.
         */
        clear_command& set_color(float r, float g, float b, float a);

        /**
         * \brief Should this clear_command clear the color buffer?
         * \param val True if the color should be cleared, otherwise false.
         * \return A reference to this clear_command object to enable method chaining.
         */
        clear_command& set_clear_color(bool val);

        /**
         * \brief Should this clear_command clear the depth buffer?
         * \param val True if the depth should be cleared, otherwise false.
         * \return A reference to this clear_command object to enable method chaining.
         */
        clear_command& set_clear_depth(bool val);
    };
} // namespace moka
