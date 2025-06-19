#include "ui/toolbar.hpp"
#include "ImGuiFileDialog.h"
#include <imgui.h>
#include <imgui_impl_SDL3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <SDL3/SDL.h>

void Toolbar::Draw(Scene& scene, std::vector<Light>& lights, Camera& camera, SceneSettings& settings)
{
    static bool openPopup = false;
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Import from .obj file"))
            {
                IGFD::FileDialogConfig config;
                config.path = "."; 
                ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose .obj File", ".obj", config);
            }
            if (ImGui::MenuItem("Save Scene and Assets"))
            {
                scene.SaveData(lights, camera, settings);
                openPopup = true;
            }
            if (ImGui::MenuItem("Export Colored ASCII"))
            {
                SDL_Event event;
                SDL_zero(event);
                event.type = SDL_EVENT_USER;
                event.user.code = 1001;
                event.user.data1 = (void*)"color";

                SDL_PushEvent(&event);
            }
            if (ImGui::MenuItem("Export Normal ASCII"))
            {
                SDL_Event event;
                SDL_zero(event);
                event.type = SDL_EVENT_USER;
                event.user.code = 1002;
                event.user.data1 = (void*)"nocolor";

                SDL_PushEvent(&event);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }    

    if (openPopup)
    {
        ImGui::OpenPopup("SavingSplash");
        openPopup = false;
    }

    if (ImGui::BeginPopupModal("SavingSplash", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar))
    {
        ImGui::Text("Saving Done!");
        if (ImGui::Button("Close")) {ImGui::CloseCurrentPopup();}
        ImGui::EndPopup();
    }
}