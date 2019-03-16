#pragma once

#include <graphics/api/graphics_api.hpp>
#include <graphics/command/graphics_command.hpp>

namespace moka
{
    /**
     * \brief Set the viewport to apply to the command list.
     */
    class viewport_command : public graphics_command
    {
    public:
        int width = 0;  /**< The width of the viewport. */
        int height = 0; /**< The height of the viewport. */
        int x = 0; /**< The x position of the lower left corner of the viewport. Initially 0. */
        int y = 0; /**< The y position of the lower left corner of the viewport. Initially 0. */

        const char* name = "viewport_command";

        virtual ~viewport_command();

        /**
         * \brief Accept a graphics_visitor object. Invoke this command using the graphics_visitor.
         * \param visitor A graphics_visitor object.
         */
        void accept(graphics_visitor& visitor) override;

        /**
         * \brief Set the rectangle of the viewport in window coordinates.
         * \param x The x position of the lower left corner of the viewport. Initially 0.
         * \param y The y position of the lower left corner of the viewport. Initially 0.
         * \param width The width of the viewport box.
         * \param height The height of the viewport box.
         * \return A reference to this viewport_command object to enable method chaining.
         */
        viewport_command& set_rectangle(int x, int y, int width, int height);

        /**
         * \brief Set the rectangle of the viewport in window coordinates.
         * \param rectangle A rectangle defining the viewport. The x and y components specify the lower left corner of the viewport. The width and height specify the width and height of the viewport.
         * \return A reference to this viewport_command object to enable method chaining.
         */
        viewport_command& set_rectangle(const rectangle& rectangle);
    };
} // namespace moka
