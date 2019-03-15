#pragma once

#include <graphics/api/graphics_api.hpp>
#include <graphics/command/graphics_command.hpp>
#include <graphics/device/graphics_visitor.hpp>

namespace moka
{
    class set_color_attachments_command : public graphics_command
    {
    public:
        std::vector<frame_attachment> attatchments;

        const char* name = "set_color_attachments_command";

        virtual ~set_color_attachments_command();

        void accept(graphics_visitor& visitor) override;

        set_color_attachments_command& set_frame_attachment(frame_attachment attatchment);
    };
} // namespace moka
