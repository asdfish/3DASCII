#include "ui/sceneSettingsMenu.hpp"
#include <string>
#include <imgui.h>
#include <imgui_impl_SDL3.h>
#include <imgui_impl_sdlrenderer3.h>

void SceneSettingsMenu::Draw(SceneSettings& settings)
{
    ImGui::Begin("Scene Settings Menu", &m_visible);
    int lightingMode = static_cast<int>(settings.lightingMode);
    ImGui::Combo("Lighting mode", &lightingMode, "Flat\0Lambertian\0\0");
    if (lightingMode == 0) {settings.lightingMode = LightingMode::FLAT;}
    if (lightingMode == 1) {settings.lightingMode = LightingMode::LAMBERTIAN;}

    int renderMode = static_cast<int>(settings.renderMode);
    ImGui::Combo("Render mode", &renderMode, "Normal\0ASCII Basic\0\0");
    if (renderMode == 0) {settings.renderMode = RenderMode::NORMAL;}
    if (renderMode == 1) {settings.renderMode = RenderMode::ASCIIBASIC;}

    float& intensityCf = settings.lightIntensityCoeff;
    ImGui::DragFloat("Light Intensity Coefficient", &intensityCf);

    float& falloffCf = settings.falloffCoeff;
    ImGui::DragFloat("Falloff Coefficient", &falloffCf);
    
    ImGui::End();
}