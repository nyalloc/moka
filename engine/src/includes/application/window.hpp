#pragma once

#include <memory>
#include <string>
#include <application/game_loop.hpp>
#include "graphics/graphics_api.hpp"

namespace moka
{
    enum class window_args
    {
        fullscreen    = 0x00000001, /**< fullscreen window */
        shown         = 0x00000002, /**< window is visible */
        hidden        = 0x00000004, /**< window is not visible */
        borderless    = 0x00000008, /**< no window decoration */
        resizable     = 0x00000010, /**< window can be resized */
        minimized     = 0x00000020, /**< window is minimized */
        maximized     = 0x00000040, /**< window is maximized */
        input_grabbed = 0x00000080, /**< window has grabbed input focus */
        input_focus   = 0x00000100, /**< window has input focus */
        mouse_focus   = 0x00000200, /**< window has mouse focus */
        mouse_capture = 0x00000400, /**< window has mouse captured (unrelated to INPUT_GRABBED) */
		always_on_top = 0x00000800, /**< window should always be above others */
    };

    using window_flags = size_t;

    struct context_handle
    {
		handle_id id;
    };

	struct window_settings
	{
		std::string name = "moka";
		glm::ivec2 resolution = { 1280, 720 };
		glm::ivec2 position = { 200, 200 };
	};

    class window
    {
    public:
        signal<> exit;
        window(const window_settings& settings);
        ~window();
        void swap_buffer() const;
        void set_size(int width, int height);
		context_handle make_context() const;
		void set_current_context(const context_handle handle);
		float aspect() const ;
    private:
        class impl;
        std::unique_ptr<impl> impl_;
    };
}
