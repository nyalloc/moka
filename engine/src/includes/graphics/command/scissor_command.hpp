#pragma once

#include <graphics/api/graphics_api.hpp>
#include <graphics/command/graphics_command.hpp>
#include <graphics/device/graphics_visitor.hpp>

namespace moka
{
    class scissor_command final : public graphics_command
    {
    public:
        int width = 0;
        int height = 0;
        int x = 0;
        int y = 0;

        virtual ~scissor_command();

        void accept(graphics_visitor& visitor) override;

        scissor_command& set_rectangle(int x, int y, int width, int height);

        scissor_command& set_rectangle(const rectangle& rectangle);
    };
} // namespace moka
