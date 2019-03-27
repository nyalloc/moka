#pragma once

#include <graphics/api/graphics_api.hpp>
#include <graphics/command/graphics_command.hpp>
#include <graphics/device/graphics_visitor.hpp>

namespace moka
{
    /**
     * \brief Set the scissor box to apply to the command list.
     */
    class scissor_command final : public graphics_command
    {
    public:
        int width = 0;  /**< The width of the scissor box. */
        int height = 0; /**< The height of the scissor box. */
        int x = 0; /**< The x position of the lower left corner of the scissor box. Initially 0. */
        int y = 0; /**< The y position of the lower left corner of the scissor box. Initially 0. */

        virtual ~scissor_command();

        /**
         * \brief Accept a graphics_visitor object. Invoke this command using the graphics_visitor.
         * \param visitor A graphics_visitor object.
         */
        void accept(graphics_visitor& visitor) override;

        /**
         * \brief Set the rectangle of the scissor box in window coordinates.
         * \param x The x position of the lower left corner of the scissor box. Initially 0.
         * \param y The y position of the lower left corner of the scissor box. Initially 0.
         * \param width The width of the scissor box.
         * \param height The height of the scissor box.
         * \return A reference to this scissor_command object to enable method chaining.
         */
        scissor_command& set_rectangle(int x, int y, int width, int height);

        /**
         * \brief Set the rectangle of the scissor box in window coordinates.
         * \param rectangle A rectangle defining the scissor box in window coordinates. The x and y components specify the lower left corner of the box. The width and height specify the width and height of the box.
         * \return A reference to this scissor_command object to enable method chaining.
         */
        scissor_command& set_rectangle(const rectangle& rectangle);
    };
} // namespace moka
