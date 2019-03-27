#include <graphics/command/viewport_command.hpp>

namespace moka
{
    viewport_command::~viewport_command() = default;

    void viewport_command::accept(graphics_visitor& visitor)
    {
        visitor.visit(*this);
    }

    viewport_command& viewport_command::set_rectangle(
        const int x, const int y, const int width, const int height)
    {
        this->width = width;
        this->height = height;
        this->x = x;
        this->y = y;
        return *this;
    }

    viewport_command& viewport_command::set_rectangle(const rectangle& rectangle)
    {
        this->width = rectangle.width;
        this->height = rectangle.height;
        this->x = rectangle.x;
        this->y = rectangle.y;
        return *this;
    }
} // namespace moka
