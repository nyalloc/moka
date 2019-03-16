#include <application/application.hpp>
#include <filesystem>
#include <glm/gtc/matrix_transform.hpp>
#include <graphics/camera/camera_mouse_controller.hpp>
#include <graphics/device/graphics_device.hpp>
#include <graphics/pbr_scene.hpp>
#include <imgui.h>
#include <imgui.hpp>

using namespace moka;

class app final : public application
{
    float fov_ = 70.0f;

    bool rotate_ = false;

    camera_mouse_controller camera_;

    imgui imgui_;

    pbr_scene scene_;

public:
    explicit app(const app_settings& settings)
        : application(settings),
          camera_(mouse_, glm::perspective(glm::radians(70.0f), window_.aspect(), 0.1f, 100.0f)),
          imgui_(window_, keyboard_, mouse_, graphics_),
          scene_(graphics_, data_path())
    {
    }

    void draw_imgui(const game_time delta_time)
    {
        imgui_.new_frame(delta_time);

        ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background
        if (ImGui::Begin(
                "Overlay",
                nullptr,
                (0 != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
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
            ImGui::SliderFloat("Gamma", &scene_.gamma, 0.0f, 10.0f, "%.3f");
            ImGui::SliderFloat("Exposure", &scene_.exposure, 0.0f, 10.0f, "%.3f");

            ImGui::SliderFloat("Camera FOV", &fov_, 20.0f, 150.0f, "%.3f degrees");
            ImGui::Checkbox("Camera Auto Rotate", &rotate_);
            ImGui::Checkbox("Draw Environment", &scene_.environment);
            ImGui::ColorEdit4("Clear Color", reinterpret_cast<float*>(&scene_.color), ImGuiColorEditFlags_PickerHueWheel);
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
