#pragma once

#include <memory>
#include <string>

namespace loki
{
    enum class window_args
    {
        fullscreen     = 0x00000001, /**< fullscreen window */
        opengl         = 0x00000002, /**< window usable with OpenGL context */
        shown          = 0x00000004, /**< window is visible */
        hidden         = 0x00000008, /**< window is not visible */
        borderless     = 0x00000010, /**< no window decoration */
        resizable      = 0x00000020, /**< window can be resized */
        minimized      = 0x00000040, /**< window is minimized */
        maximized      = 0x00000080, /**< window is maximized */
        input_grabbed  = 0x00000100, /**< window has grabbed input focus */
        input_focus    = 0x00000200, /**< window has input focus */
        mouse_focus    = 0x00000400, /**< window has mouse focus */
        window_foreign = 0x00000800, /**< window not created by SDL */
        allow_high_dpi = 0x00002000, /**< window should be created in high-DPI mode if supported */
        mouse_capture  = 0x00004000, /**< window has mouse captured (unrelated to INPUT_GRABBED) */
        always_on_top  = 0x00008000, /**< window should always be above others */
        skip_taskbar   = 0x00010000, /**< window should not be added to the taskbar */
        utility        = 0x00020000, /**< window should be treated as a utility window */
        tooltip        = 0x00040000, /**< window should be treated as a tooltip */
        popup_menu     = 0x00080000, /**< window should be treated as a popup menu */
        vulkan         = 0x10000000  /**< window usable for Vulkan surface */
    };

    using window_flags = size_t;

    class window
    {
    public:
        window(const std::string& title, window_flags flags);
        ~window() = default;
    private:
        class window_impl;
        std::unique_ptr<window_impl> impl_;
    };
}
