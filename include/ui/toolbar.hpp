#pragma once

#include "Scene.hpp"

class Toolbar
{
    public:
        void Draw(Scene& scene, std::vector<Light>& lights, Camera& camera, SceneSettings& settings);
        void SetVisible(bool visible) { m_visible = visible; }
        bool GetVisible() const { return m_visible; }
    private:
        bool m_visible = true;
};