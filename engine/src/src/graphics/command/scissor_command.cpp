#include <graphics/command/scissor_command.hpp>

namespace moka
{
    scissor_command::~scissor_command() = default;

    void scissor_command::accept(graphics_visitor& visitor)
    {
        visitor.visit(*this);
    }

    scissor_command& scissor_command::set_rectangle(int x, int y, int width, int height)
    {
        this->width = width;
        this->height = height;
        this->x = x;
        this->y = y;
        return *this;
    }

    scissor_command& scissor_command::set_rectangle(const rectangle& rectangle)
    {
        this->width = rectangle.width;
        this->height = rectangle.height;
        this->x = rectangle.x;
        this->y = rectangle.y;
        return *this;
    }
} // namespace moka
