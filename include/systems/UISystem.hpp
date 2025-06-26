#pragma once

#include "ecss-templates/system.hpp"
#include "managers/Coordinator.hpp"
#include "managers/AssetManager.hpp"

class UISystem : public System
{
    public:
        void Update();

    private:
        Entity m_selectedEntity;

        void DrawAssetBrowser();
        void DrawInspectorMenu();
        void DrawMenuBar();
        void DrawSceneSettingsMenu();
        void DrawSceneViewer();

        void HandleInputLogic();
};

inline float FastSmoothFalloff(float x, float minBound, float maxBound)
{
    float mid = 0.5f * (minBound + maxBound);
    float range = 0.5f * (maxBound - minBound);
    float t = std::abs(x - mid) / range;
    t = std::min(t, 1.0f);            // Clamp to [0,1]
    return (1.0f - t) * (1.0f - t);   // Quadratic falloff
}