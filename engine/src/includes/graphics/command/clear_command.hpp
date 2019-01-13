#pragma once

#include <glm/detail/type_vec4.hpp>
#include <graphics/command/graphics_command.hpp>

namespace moka
{
    class clear_command final : public graphics_command
    {
    public:
        glm::vec4 color;
        bool clear_depth = false;
        bool clear_color = false;

        virtual ~clear_command();

        void accept(graphics_visitor& visitor) override;

        clear_command& set_color(const glm::vec4& color);

        clear_command& set_color(float r, float g, float b, float a);

        clear_command& set_clear_color(bool val);

        clear_command& set_clear_depth(bool val);
    };
} // namespace moka
