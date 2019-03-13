#pragma once

#include <graphics/device/graphics_device.hpp>
#include <graphics/material/material.hpp>
#include <imgui.h>

namespace moka
{
    class keyboard;
    class mouse;

    class imgui
    {
        window& window_;
        keyboard& keyboard_;
        mouse& mouse_;

        material_handle material_;

        index_buffer index_buffer_;
        vertex_buffer vertex_buffer_;

        graphics_device& graphics_device_;

        texture font_atlas_{};

    public:
        imgui(window& window, keyboard& keyboard, mouse& mouse, graphics_device& graphics_device);

        void new_frame(float data_time) const;

        command_list draw();
    };
} // namespace moka
