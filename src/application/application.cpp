#include <application/application.hpp>
#include <imgui.h>
#include <imgui_impl_sdl_gl3.h>
#include <cstdio>
#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include "game_loop/game_loop.hpp"

namespace loki
{
    application::application(int argc, char* argv[])
    {
        for (auto i = 1; i < argc; ++i)
        {   
            // simply print command line arguments (for now)
            std::cout << argv[i] << std::endl;
        }

        // Setup SDL
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
        {
            SDL_DisplayMode current;
            // Setup window
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
            SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
            SDL_GetCurrentDisplayMode(0, &current);
            window = SDL_CreateWindow("loki editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
            gl_context = SDL_GL_CreateContext(window);
            SDL_GL_SetSwapInterval(1); // Enable vsync

            glewExperimental = GL_TRUE;
            if (glewInit() != GLEW_OK)
            {
                std::cout << "Fatal Error: GLEW not initialized correctly." << std::endl;
                std::abort();
            }

            // Setup Dear ImGui binding
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
            ImGui_ImplSdlGL3_Init(window);

            // Setup style
            ImGui::StyleColorsDark();
        }
        else
        {
            //ImGui::StyleColorsClassic();
            printf("Error: %s\n", SDL_GetError());
            running = true;
        }
    }

    application::~application()
    {
        ImGui_ImplSdlGL3_Shutdown();
        ImGui::DestroyContext();

        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    int application::run()
    {
        auto update([this](game_time time)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                ImGui_ImplSdlGL3_ProcessEvent(&event);
                if (event.type == SDL_QUIT)
                    running = false;
            }
        });

        auto draw([this](game_time time)
        {
            ImGui_ImplSdlGL3_NewFrame(window);

            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);

            ImGui::ShowDemoWindow(&show_demo_window);

            // Rendering
            glViewport(0, 0, static_cast<int>(ImGui::GetIO().DisplaySize.x), static_cast<int>(ImGui::GetIO().DisplaySize.y));
            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui::Render();
            ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(window);
        });

        game_loop game_loop(draw, update, running);

        return 0;
    }
}
