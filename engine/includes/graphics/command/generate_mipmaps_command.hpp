#pragma once

#include <graphics/command/graphics_command.hpp>
#include <graphics/texture_handle.hpp>

namespace moka
{
    /**
     * \brief Generate mipmaps for a specified texture.
     */
    class generate_mipmaps_command final : public graphics_command
    {
    public:
        texture_handle texture; /**< The texture object to generate mipmaps for. */

        virtual ~generate_mipmaps_command();

        /**
         * \brief Accept a graphics_visitor object. Invoke this command using the graphics_visitor.
         * \param visitor A graphics_visitor object.
         */
        void accept(graphics_visitor& visitor) override;

        /**
         * \brief Set the texture object to generate mipmaps for.
         * \param texture The texture object to generate mipmaps for.
         * \return A reference to this generate_mipmaps_command object to enable method chaining.
         */
        generate_mipmaps_command& set_texture(texture_handle texture);
    };
} // namespace moka
