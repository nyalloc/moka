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
	public:
		imgui(window& window
			, keyboard& keyboard
			, mouse& mouse);

		void update(const float data_time);

		void draw(ImDrawData* draw_data);
	};
}