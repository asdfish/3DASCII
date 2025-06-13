#include "ui/sceneViewer.hpp"
#include <imgui.h>
#include <imgui_impl_SDL3.h>
#include <imgui_impl_sdlrenderer3.h>

#include <iostream>

void SceneViewer::Draw(Scene& scene, SceneObject*& selectedObject)
{
    ImGui::Begin("Scene Viewer", &m_visible);

    ImGui::Text("Scene Objects:");
    if (ImGui::BeginTable("SceneObjects", 1))
    {
        ImGui::TableSetupColumn("Objects: ", ImGuiTableColumnFlags_WidthStretch);

        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f)); 
        for (auto &obj : scene.GetSceneObjects())
        {
            ImGuiStyle style;
            
            ImGui::TableNextColumn();

            if (selectedObject==&obj)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.5f, 0.8f, 1.0f));
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.5f, 0.8f, 0.0f));
            }
            
            if (ImGui::Button((obj.GetName() + "##" + obj.GetID()).c_str(), ImVec2(-FLT_MIN, 0)))
            {
                selectedObject = &obj;
            }

            ImGui::PopStyleColor();
            
        }
        ImGui::PopStyleVar();
        ImGui::EndTable();
    }
    
    ImGui::End();
}