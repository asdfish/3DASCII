#pragma once

#include "SceneSettings.hpp"

class SceneSettingsMenu
{
    public:
        void Draw(SceneSettings& settings);
        void SetVisible(bool visible) { m_visible = visible; }
        bool GetVisible() const { return m_visible; }
    private:
        bool m_visible = true;
};