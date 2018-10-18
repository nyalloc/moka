#pragma once

#include <imgui.hpp>
#include <window/window.hpp>
#include <input/keyboard.hpp>
#include <input/mouse.hpp>
#include <graphics/material_builder.hpp>
#include <graphics/draw_call_builder.hpp>

namespace moka
{
	imgui::imgui(window& window
		, keyboard& keyboard
		, mouse& mouse
		, graphics_device& graphics_device)
		: window_(window)
		, keyboard_(keyboard)
		, mouse_(mouse)
		, graphics_device_(graphics_device)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		
		ImFont* font = io.Fonts->AddFontDefault();

		ImGui::StyleColorsDark();

		io.DisplaySize.x = 1280;
		io.DisplaySize.y = 720;

		io.Fonts->GetTexDataAsRGBA32(&font_data, &font_width, &font_height, &font_components);
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
		ImGuiIO& io = ImGui::GetIO();
		int fb_width = (int)(draw_data->DisplaySize.x * io.DisplayFramebufferScale.x);
		int fb_height = (int)(draw_data->DisplaySize.y * io.DisplayFramebufferScale.y);

		if (fb_width <= 0 || fb_height <= 0)
		{
			return;
		}

		draw_data->ScaleClipRects(io.DisplayFramebufferScale);

		float L = draw_data->DisplayPos.x;
		float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
		float T = draw_data->DisplayPos.y;
		float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

		const float ortho_projection[4][4] =
		{
			{ 2.0f / (R - L), 0.0f, 0.0f, 0.0f },
			{ 0.0f, 2.0f / (T - B), 0.0f, 0.0f },
			{ 0.0f,	0.0f, -1.0f, 0.0f },
			{ (R + L) / (L - R), (T + B) / (B - T), 0.0f, 1.0f },
		};

		auto layout = vertex_layout::builder()
			.add_attribute(0, attribute_type::float32, 2, false, sizeof(ImDrawVert), IM_OFFSETOF(ImDrawVert, pos))
			.add_attribute(1, attribute_type::float32, 2, false, sizeof(ImDrawVert), IM_OFFSETOF(ImDrawVert, uv))
			.add_attribute(2, attribute_type::uint32,  4, true,  sizeof(ImDrawVert), IM_OFFSETOF(ImDrawVert, col))
			.build();

		for (int n = 0; n < draw_data->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			const ImDrawIdx* idx_buffer_offset = 0;

			auto vertex_handle = graphics_device_.create_vertex_buffer(cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), layout);
			auto index_handle  = graphics_device_.create_index_buffer(cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));

			for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
			{
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
				if (pcmd->UserCallback)
				{
					// User callback (registered via ImDrawList::AddCallback)
					pcmd->UserCallback(cmd_list, pcmd);
				}
				else
				{
					//ImVec4 clip_rect = ImVec4(pcmd->ClipRect.x - pos.x, pcmd->ClipRect.y - pos.y, pcmd->ClipRect.z - pos.x, pcmd->ClipRect.w - pos.y);
					//if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
					//{
						//// Apply scissor/clipping rectangle
						//glScissor((int)clip_rect.x, (int)(fb_height - clip_rect.w), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));

						//// Bind texture, Draw
						//glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
						//glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
					//}

					//graphics_device_.draw()
					//	.set_vertex_buffer(vertex_handle, 0, cmd_list->VtxBuffer.Size)
					//	.set_index_buffer(index_handle, cmd_list->IdxBuffer.Size);
				}
				idx_buffer_offset += pcmd->ElemCount;
			}
		}

		// Restore modified GL state
	}
}