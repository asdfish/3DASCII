#include "ui/assetBrowser.hpp"
#include <imgui.h>
#include <imgui_impl_SDL3.h>
#include <imgui_impl_sdlrenderer3.h>

void AssetBrowser::Draw(Scene &scene, SceneObject*& selectedObject)
{
    ImGui::Begin("Asset Browser", &m_visible);

        ImGui::Text("Assets:");
        for (const auto &obj : scene.GetAssetObjects())
        {
            ImGui::Text("%s", obj.GetName().c_str());
            if (ImGui::Button(("Add##" + obj.GetName()).c_str()))
            {
                scene.AddObjectToScene(obj.GetID());
                selectedObject = nullptr;
            }
        }

    ImGui::End();
}