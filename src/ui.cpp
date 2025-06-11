#include "ui.hpp"

void DisplayUI()
{
}

void ModelUI(const Model& model)
{
    ImGui::Begin("Model UI");
    ImGui::Text("Model Name: %s", model.GetName().c_str());
    ImGui::Text("Number of Vertices: %zu", model.GetVerts().size());
    ImGui::Text("Number of Faces: %zu", model.GetFaceIndices().size() / 3);
    ImGui::End();
}