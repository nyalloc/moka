#pragma once

#include <imgui.h>
#include <graphics/material.hpp>

namespace moka
{
	class window;
	class keyboard;
	class mouse;

	class imgui
	{
		window& window_;
		keyboard& keyboard_;
		mouse& mouse_;

		material material_;

		index_buffer_handle index_buffer_;
		vertex_buffer_handle vertex_buffer_;

		graphics_device& graphics_device_;

		unsigned char* font_data;
		int font_width;
		int font_height;
		int font_components;
	public:
		imgui(window& window
			, keyboard& keyboard
			, mouse& mouse
			, graphics_device& graphics_device);

		void update(const float data_time);

		void draw(ImDrawData* draw_data);
	};
}