#pragma once

#include <imgui.hpp>
#include <window/window.hpp>
#include <input/keyboard.hpp>
#include <input/mouse.hpp>

namespace moka
{
	imgui::imgui(window& window
		, keyboard& keyboard
		, mouse& mouse)
		: window_(window)
		, keyboard_(keyboard)
		, mouse_(mouse)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		ImGui::StyleColorsDark();

		io.DisplaySize.x = 1280;
		io.DisplaySize.y = 720;
	}

	void imgui::update(const float data_time)
	{
		ImGuiIO& io = ImGui::GetIO();

		io.DisplaySize.x = 1280;
		io.DisplaySize.y = 720;

		auto mouse_state = mouse_.get_state();
		auto position = mouse_state.get_position();
		auto motion = mouse_state.get_motion();

		io.MousePos.x = position.x;
		io.MousePos.y = position.y;
	}

	void imgui::draw(ImDrawData* draw_data)
	{

	}
}