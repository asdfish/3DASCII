#include "ui/sceneViewer.hpp"
#include <imgui.h>
#include <imgui_impl_SDL3.h>
#include <imgui_impl_sdlrenderer3.h>

#include <iostream>

void SceneViewer::Draw(Scene& scene, SceneObject*& selectedObject, std::vector<Light>& lights, Light*& selectedLight, Camera& camera, Camera*& selectedCamera, std::optional<Light>& copiedLight, std::optional<SceneObject>& copiedObject)
{
    ImGui::Begin("Scene Viewer", &m_visible);

    //Popup for adding lights
    if(ImGui::BeginPopupContextWindow("SceneViewerContextMenu", ImGuiPopupFlags_MouseButtonRight))
    {
        if (ImGui::Button("Add New Light"))
        {
            lights.emplace_back(Light(float3(), 100.f));
            selectedObject = nullptr;
            selectedLight = &lights.back();
            selectedCamera = nullptr;
        }
        ImGui::EndPopup();
    }

    //Camera
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
    if (selectedCamera==&camera)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.5f, 0.8f, 1.0f));
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.5f, 0.8f, 0.0f));
    }

    if (ImGui::Button("Camera", ImVec2(-FLT_MIN, 0)))
    {
        selectedObject = nullptr;
        selectedLight = nullptr;
        selectedCamera = &camera;
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    //Scene Objects
    ImGui::Separator();
    //ImGui::Text("Scene Objects:");
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
                selectedLight = nullptr;
                selectedCamera = nullptr;
            }

            ImGui::PopStyleColor();
            
        }
        ImGui::PopStyleVar();
        ImGui::EndTable();
    }



    //Lights
    ImGui::Separator();
    //ImGui::Text("Lights: ");
    if (ImGui::BeginTable("Lights", 1))
    {
        ImGui::TableSetupColumn("Lights:  ", ImGuiTableColumnFlags_WidthStretch);

        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f)); 
        for (auto &light : lights)
        {
            ImGuiStyle style;
            
            ImGui::TableNextColumn();

            if (selectedLight==&light)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.5f, 0.8f, 1.0f));
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.5f, 0.8f, 0.0f));
            }
            
            if (ImGui::Button((light.GetName() + "##" + light.GetID()).c_str(), ImVec2(-FLT_MIN, 0)))
            {
                selectedObject = nullptr;
                selectedLight = &light;
                selectedCamera = nullptr;
            }

            ImGui::PopStyleColor();
            
        }
        ImGui::PopStyleVar();
        ImGui::EndTable();
    }
    if(ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiKey_C))
    {
        if (selectedObject != nullptr)
        {
            copiedObject = *selectedObject;
            copiedLight = std::nullopt;

            copiedObject->ChangeID();
        }
        if (selectedLight != nullptr)
        {
            copiedObject = std::nullopt;
            copiedLight = *selectedLight;

            copiedLight->ChangeID();
        }
    }
    if(ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiKey_V))
    {
        if(copiedObject.has_value())
        {
            scene.AddObjectToScene(*copiedObject);
            selectedObject = nullptr;
            selectedLight = nullptr;
            selectedCamera = nullptr;
            
        }
        if(copiedLight.has_value())
        {
            lights.push_back(*copiedLight);
            selectedObject = nullptr;
            selectedLight = nullptr;
            selectedCamera = nullptr;
        }
    }
    ImGui::End();
}