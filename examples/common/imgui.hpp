#pragma once

#include <imgui.h>
#include <graphics/material.hpp>
#include <graphics/graphics_device.hpp>

namespace moka
{
	class keyboard;
	class mouse;

	class imgui
	{
		window& window_;
		keyboard& keyboard_;
		mouse& mouse_;

		material material_;

		index_buffer index_buffer_;
		vertex_buffer vertex_buffer_;

		graphics_device& graphics_device_;

		texture font_atlas_{};
	public:
		imgui(window& window
			, keyboard& keyboard
			, mouse& mouse
			, graphics_device& graphics_device);

		void new_frame(float data_time);

		command_list draw();
	};
}