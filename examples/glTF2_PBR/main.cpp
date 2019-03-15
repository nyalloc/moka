#include <application/application.hpp>
#include <application/open_file.hpp>
#include <asset_importer/model_importer.hpp>
#include <filesystem>
#include <graphics/camera/camera_mouse_controller.hpp>
#include <graphics/color.hpp>
#include <graphics/device/graphics_device.hpp>
#include <graphics/material/material_builder.hpp>
#include <graphics/model.hpp>
#include <graphics/pbr.hpp>
#include <graphics/scene.hpp>
#include <imgui.h>
#include <imgui.hpp>

using namespace moka;

class app final : public application
{
    float fov_ = 70.0f;

    bool rotate_ = false;

    camera_mouse_controller camera_;

    imgui imgui_;

    scene scene_;

public:
    explicit app(const app_settings& settings)
        : application(settings),
          scene_(graphics_, data_path()),
          camera_(mouse_, glm::perspective(glm::radians(70.0f), window_.aspect(), 0.1f, 100.0f)),
          imgui_(window_, keyboard_, mouse_, graphics_)
    {
    }

    void draw_imgui(const game_time delta_time)
    {
        imgui_.new_frame(delta_time);

        auto& io = ImGui::GetIO();

        ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background
        if (ImGui::Begin(
                "Overlay",
                nullptr,
                (0 != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize |
                    ImGuiWindowFlags_NoSavedSettings |
                    ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
        {
            ImGui::Text(
                "Real-Time Physically-Based Rendering\n"
                "Stuart Adams\n"
                "@stuartdadams\n"
                "www.stuartdadams.co.uk");
        }
        ImGui::End();

        ImGui::Begin("Render Settings");
        {
            ImGui::SliderFloat("Gamma", &scene_.gamma_, 0.0f, 10.0f, "%.3f");
            ImGui::SliderFloat(
                "Exposure", &scene_.exposure_, 0.0f, 10.0f, "%.3f");

            ImGui::SliderFloat(
                "Camera FOV", &fov_, 20.0f, 150.0f, "%.3f degrees");
            ImGui::Checkbox("Camera Auto Rotate", &rotate_);
            ImGui::Checkbox("Draw Environment", &scene_.environment_);
            ImGui::ColorEdit4(
                "Clear Color", reinterpret_cast<float*>(&scene_.color_), ImGuiColorEditFlags_PickerHueWheel);
        }
        ImGui::End();

        graphics_.submit_and_swap(imgui_.draw());
    }

    void draw(const game_time delta_time) override
    {
        scene_.draw(camera_, window_.get_viewport());
        draw_imgui(delta_time);
    }

    void update(const game_time delta_time) override
    {
        camera_.set_perspective(glm::radians(fov_), window_.aspect());
        camera_.set_auto_rotate(rotate_);
        camera_.update(delta_time);
    }

    std::filesystem::path data_path() override
    {
        // cmake-defined macro points to example project asset folder relative
        // to source. A real application could point this wherever it wanted.
        std::filesystem::path result{MOKA_ASSET_PATH};
        return result.lexically_normal();
    }
};

int main()
{
    app_settings settings{};
    settings.window_settings.resolution = {1600, 900};
    settings.window_settings.fullscreen = false;

    return app{settings}.run();
}
