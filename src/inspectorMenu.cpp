#include "ui/inspectorMenu.hpp"
#include <imgui.h>
#include <imgui_impl_SDL3.h>
#include <imgui_impl_sdlrenderer3.h>

void InspectorMenu::Draw(SceneObject*& selectedObject)
{
    ImGui::Begin("Inspector", &m_visible);
    if (selectedObject == nullptr)
    {
        ImGui::Text("No object selected.");
    }
    else
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
    ImGui::End();
}
