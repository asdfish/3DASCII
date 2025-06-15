#include "ui/inspectorMenu.hpp"
#include <imgui.h>
#include <imgui_impl_SDL3.h>
#include <imgui_impl_sdlrenderer3.h>

#include <iostream>

void InspectorMenu::Draw(SceneObject*& selectedObject, Light*& selectedLight, Camera*& selectedCamera)
{

    int count = (selectedObject != nullptr)+(selectedLight != nullptr)+(selectedCamera != nullptr);
    ImGui::Begin("Inspector", &m_visible);
    if (count == 0)
    {
        ImGui::Text("No object selected.");
    }
    else if (count > 1)
    {
        std::cout << "ERROR: TWO ITEMS ARE ASSIGNED AT ONCE\n";
    }
    /* Object */
    else if (selectedObject)
    {
        ImGui::Text("Name: %s", selectedObject->GetName().c_str());
        ImGui::Text("ID: %s", selectedObject->GetID().c_str());

        ImGui::Separator();

        // Display Transform
        ImGui::Text("Transform:");
        Transform &transform = selectedObject->GetTransform();
        float3 position = transform.GetPos();
        float3 rotation = transform.GetRot();
        ImGui::DragFloat3("Position", &position.x, 0.05f);
        ImGui::DragFloat3("Rotation", &rotation.x, 0.1f, 0.0f, 2.f*M_PI, "%.2f rad", ImGuiSliderFlags_WrapAround);

        // Update the transform with new values
        transform.SetPos(position);
        transform.SetRot(rotation);      

        ImGui::Separator();

        // Display Model Details
        Model &model = selectedObject->GetModel();
        ImGui::Text("Model:");
        ImGui::Text("Vertices: %d", model.GetLocalVerts().size());
        ImGui::Text("Faces: %d", model.GetFaceIndices().size() / 3);

        
    }

    /* Light */
    else if (selectedLight)
    {
        ImGui::Text("Name: %s", selectedLight->GetName().c_str());
        ImGui::Text("ID: %s", selectedLight->GetID().c_str());

        // Position
        ImGui::Text("Transform:");
        float3 position = selectedLight->GetPosition();
        ImGui::DragFloat3("Position", &position.x, 0.05f);
        selectedLight->SetPosition(position);

        //Intensity
        ImGui::Text("Intensity:");
        float intensity = selectedLight->GetIntensity();
        ImGui::DragFloat("Intensity", &intensity, 0.05f);
        selectedLight->SetIntensity(intensity);
    }

    /* Camera */
    else if (selectedCamera)
    {
        ImGui::Text("Camera");

        // Display Transform
        ImGui::Text("Transform:");
        Transform &transform = selectedCamera->GetModifiableTransform();
        float3 position = transform.GetPos();
        float3 rotation = transform.GetRot();
        ImGui::DragFloat3("Position", &position.x, 0.05f);
        ImGui::DragFloat3("Rotation", &rotation.x, 0.1f, 0.0f, 2.f*M_PI, "%.2f rad", ImGuiSliderFlags_WrapAround);

        // Update the transform with new values
        transform.SetPos(position);
        transform.SetRot(rotation);     
    }
    else
    {
        std::cout<<"ERROR: TWO ITEMS ARE ASSIGNED AT ONCE";
    }

    ImGui::End();
}
