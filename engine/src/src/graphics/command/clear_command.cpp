#include <graphics/command/clear_command.hpp>
#include <graphics/device/graphics_visitor.hpp>

namespace moka
{
    clear_command::~clear_command() = default;

    void clear_command::accept(graphics_visitor& visitor)
    {
        visitor.visit(*this);
    }

    clear_command& clear_command::set_color(const glm::vec4& color)
    {
        this->color = color;
        return *this;
    }

    clear_command& clear_command::set_color(const float r, const float g, const float b, const float a)
    {
        color = {r, g, b, a};
        return *this;
    }

    clear_command& clear_command::set_clear_color(const bool val)
    {
        clear_color = val;
        return *this;
    }

    clear_command& clear_command::set_clear_depth(const bool val)
    {
        clear_depth = val;
        return *this;
    }
} // namespace moka
