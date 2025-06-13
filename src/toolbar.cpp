#include "ui/toolbar.hpp"
#include "ImGuiFileDialog.h"
#include <imgui.h>
#include <imgui_impl_SDL3.h>
#include <imgui_impl_sdlrenderer3.h>

void Toolbar::Draw(Scene& scene)
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
                scene.SaveData();
                openPopup = true;
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