#include <graphics/command/set_color_attachments_command.hpp>

namespace moka
{
    set_color_attachments_command::~set_color_attachments_command() = default;

    set_color_attachments_command& set_color_attachments_command::set_frame_attachment(frame_attachment attatchment)
    {
        attatchments.emplace_back(attatchment);
        return *this;
    }

    void set_color_attachments_command::accept(graphics_visitor& visitor)
    {
        visitor.visit(*this);
    }
} // namespace moka
