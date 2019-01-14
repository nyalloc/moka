#pragma once

#include <graphics/api/graphics_api.hpp>
#include <graphics/command/graphics_command.hpp>
#include <graphics/device/graphics_visitor.hpp>

namespace moka
{
    class viewport_command : public graphics_command
    {
    public:
        int width = 0;
        int height = 0;
        int x = 0;
        int y = 0;

        const char* name = "viewport_command";

        virtual ~viewport_command();

        void accept(graphics_visitor& visitor) override;

        viewport_command& set_rectangle(int x, int y, int width, int height);

        viewport_command& set_rectangle(const rectangle& rectangle);
    };
} // namespace moka
