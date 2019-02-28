
#include <SDL_mouse.h>
#include <application/window.hpp>
#include <graphics/buffer/vertex_attribute.hpp>
#include <graphics/material/material_builder.hpp>
#include <imgui.hpp>
#include <input/keyboard.hpp>
#include <input/mouse.hpp>
#include <iostream>

namespace moka
{
    void theme()
    {
        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
        colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.21f, 0.22f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.46f, 0.47f, 0.48f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.70f, 0.70f, 0.70f, 0.31f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.48f, 0.50f, 0.52f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.73f, 0.60f, 0.15f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
        colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    }

    imgui::imgui(window& window, keyboard& keyboard, mouse& mouse, graphics_device& graphics_device)
        : window_(window), keyboard_(keyboard), mouse_(mouse), graphics_device_(graphics_device)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        auto& io = ImGui::GetIO();

        if (!io.Fonts->AddFontDefault())
        {
            std::cout << "Null ptr to default font" << std::endl;
        }

        theme();

        io.DisplaySize.x = window.get_size().x;
        io.DisplaySize.y = window.get_size().y;

        const std::string vertex_shader =
            R"(
			#version 330
			uniform mat4 u_projection;
			layout (location = 0) in vec2 pos;
			layout (location = 1) in vec2 uv;
			layout (location = 2) in vec4 color;
			out vec2 frag_uv;
			out vec4 frag_color;
			void main()
			{
				frag_uv = uv;
				frag_color = color;
				gl_Position = u_projection*vec4(pos, 0.0, 1.0);
			} 
		)";

        const std::string fragment_shader =
            R"(
			#version 330
			uniform sampler2D u_tex0;
			in vec2 frag_uv;
			in vec4 frag_color;
			out vec4 color;
			void main() 
			{
				color = texture(u_tex0, frag_uv)*frag_color;
			}
		)";

        std::map<std::string, program> shaders;

        material::builder builder(graphics_device_, shaders);

        builder.set_fragment_shader(fragment_shader);
        builder.set_vertex_shader(vertex_shader);

        builder.set_blend_enabled(true);
        builder.set_blend_equation(blend_equation::func_add);
        builder.set_blend_function(
            blend_function_factor::src_alpha, blend_function_factor::one_minus_src_alpha);
        builder.set_culling_enabled(false);
        builder.set_polygon_mode(face::front_and_back, polygon_draw_mode::fill);

        builder.set_depth_test_enabled(false);
        builder.set_scissor_test_enabled(true);

        material_ = builder.build();

        unsigned char* pixels;
        int width, height;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

        const auto font_atlas = graphics_device_.build_texture()
                                    .add_image_data(
                                        image_target::texture_2d,
                                        0,
                                        device_format::rgba,
                                        width,
                                        height,
                                        0,
                                        host_format::rgba,
                                        pixel_type::uint8,
                                        pixels)
                                    .set_wrap_s(wrap_mode::clamp_to_edge)
                                    .set_wrap_t(wrap_mode::clamp_to_edge)
                                    .set_mipmaps(true)
                                    .set_min_filter(min_filter::linear)
                                    .set_mag_filter(mag_filter::linear)
                                    .build();

        io.Fonts->TexID =
            reinterpret_cast<ImTextureID>(static_cast<intptr_t>(font_atlas.id));

        auto pos_size = IM_OFFSETOF(ImDrawVert, pos);
        auto uv_size = IM_OFFSETOF(ImDrawVert, uv);
        auto col_size = IM_OFFSETOF(ImDrawVert, col);

        vertex_layout layout{
            vertex_attribute{0, attribute_type::float32, 2, false, sizeof(ImDrawVert), pos_size},
            vertex_attribute{1, attribute_type::float32, 2, false, sizeof(ImDrawVert), uv_size},
            vertex_attribute{2, attribute_type::uint8, 4, true, sizeof(ImDrawVert), col_size}};

        index_buffer_ = graphics_device_.make_index_buffer(
            nullptr, 0, index_type::uint16, buffer_usage::stream_draw);

        vertex_buffer_ = graphics_device_.make_vertex_buffer(
            nullptr, 0, std::move(layout), buffer_usage::stream_draw);
    }

    void imgui::new_frame(const float data_time) const
    {
        auto& io = ImGui::GetIO();

        io.DisplaySize.x = window_.get_size().x;
        io.DisplaySize.y = window_.get_size().y;

        const auto& mouse_state = mouse_.get_state();
        const auto& position = mouse_state.get_position();
        const auto& motion = mouse_state.get_motion();
        const auto& scroll = mouse_state.get_scroll();

        io.MousePos.x = static_cast<float>(position.x);
        io.MousePos.y = static_cast<float>(position.y);

        io.MouseWheel = scroll.y;

        IM_ASSERT(io.Fonts->IsBuilt());

        const auto size = window_.get_size();
        const auto display_size = window_.get_drawable_size();
        io.DisplaySize = ImVec2(static_cast<float>(size.x), static_cast<float>(size.y));
        io.DisplayFramebufferScale = ImVec2(
            size.x > 0 ? static_cast<float>(display_size.x) / size.x : 0,
            size.y > 0 ? static_cast<float>(display_size.y) / size.y : 0);

        io.DeltaTime = data_time;

        io.MouseDown[0] = mouse_state.is_button_down(mouse_button::left);
        io.MouseDown[1] = mouse_state.is_button_down(mouse_button::right);
        io.MouseDown[2] = mouse_state.is_button_down(mouse_button::middle);

        ImGui::NewFrame();
    }

    command_list imgui::draw()
    {
        command_list list;

        ImGui::Render();

        auto& buff = list.make_command_buffer();

        auto* draw_data = ::ImGui::GetDrawData();

        if (!draw_data)
        {
            return {};
        }

        auto& io = ::ImGui::GetIO();

        const auto fb_width = static_cast<int>(
            draw_data->DisplaySize.x * io.DisplayFramebufferScale.x);
        const auto fb_height = static_cast<int>(
            draw_data->DisplaySize.y * io.DisplayFramebufferScale.y);

        if (fb_width <= 0 || fb_height <= 0)
        {
            return {};
        }

        draw_data->ScaleClipRects(io.DisplayFramebufferScale);

        buff.viewport().set_rectangle(0, 0, fb_width, fb_height);

        const auto l = draw_data->DisplayPos.x;
        const auto r = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
        const auto t = draw_data->DisplayPos.y;
        const auto b = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

        const glm::mat4 proj = {2.0f / (r - l),
                                0.0f,
                                0.0f,
                                0.0f,
                                0.0f,
                                2.0f / (t - b),
                                0.0f,
                                0.0f,
                                0.0f,
                                0.0f,
                                -1.0f,
                                0.0f,
                                (r + l) / (l - r),
                                (t + b) / (b - t),
                                0.0f,
                                1.0f};

        material_["u_projection"] = proj;

        const auto pos = draw_data->DisplayPos;
        for (auto n = 0; n < draw_data->CmdListsCount; ++n)
        {
            const ImDrawList* cmd_list = draw_data->CmdLists[n];
            uint32_t idx_buffer_offset = 0;

            buff.fill_vertex_buffer().set_buffer(
                vertex_buffer_,
                cmd_list->VtxBuffer.Data,
                static_cast<size_t>(cmd_list->VtxBuffer.Size) * sizeof(ImDrawVert));

            buff.fill_index_buffer().set_buffer(
                index_buffer_,
                cmd_list->IdxBuffer.Data,
                static_cast<size_t>(cmd_list->IdxBuffer.Size) * sizeof(ImDrawIdx));

            for (auto cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
            {
                auto cmd = &cmd_list->CmdBuffer[cmd_i];

                if (cmd->UserCallback)
                {
                    cmd->UserCallback(cmd_list, cmd);
                    continue;
                }

                const auto clip_rect = ImVec4(
                    cmd->ClipRect.x - pos.x,
                    cmd->ClipRect.y - pos.y,
                    cmd->ClipRect.z - pos.x,
                    cmd->ClipRect.w - pos.y);
                if (clip_rect.x < fb_width && clip_rect.y < fb_height &&
                    clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
                {
                    buff.scissor().set_rectangle(
                        static_cast<int>(clip_rect.x),
                        static_cast<int>(fb_height - clip_rect.w),
                        static_cast<int>(clip_rect.z - clip_rect.x),
                        static_cast<int>(clip_rect.w - clip_rect.y));

                    if (cmd->TextureId)
                    {
                        const texture handle{static_cast<uint16_t>(
                            reinterpret_cast<intptr_t>(cmd->TextureId))};
                        material_["u_tex0"] = handle;
                    }

                    buff.clear().set_clear_depth(true);

                    buff.draw()
                        .set_vertex_buffer(vertex_buffer_)
                        .set_index_count(cmd->ElemCount)
                        .set_index_buffer(index_buffer_)
                        .set_index_type(index_type::uint16)
                        .set_index_buffer_offset(sizeof(ImDrawIdx) * idx_buffer_offset)
                        .set_material(material_);
                }
                idx_buffer_offset += cmd->ElemCount;
            }
        }

        return list;
    }
} // namespace moka
