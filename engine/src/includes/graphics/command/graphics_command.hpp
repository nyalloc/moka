#pragma once

#include <memory>

namespace moka
{
    class graphics_visitor;

    /**
     * \brief Base type for graphics commands
     */
    class graphics_command
    {
    public:
        /**
         * \brief Accept a graphics_visitor object. Invoke this command using the graphics_visitor.
         * \param visitor A graphics_visitor object.
         */
        virtual void accept(graphics_visitor& visitor) = 0;

        virtual ~graphics_command() = default;
    };

    /**
     * \brief An owning pointer to a graphics command.
     */
    using graphics_command_ptr = std::unique_ptr<graphics_command>;
} // namespace moka
