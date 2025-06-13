#pragma once

#include "Scene.hpp"
#include "SceneObject.hpp"

class InspectorMenu
{
    public:
        void Draw(SceneObject*& selectedObject);
        void SetVisible(bool visible) { m_visible = visible; }
        bool GetVisible() const { return m_visible; }
    private:
        bool m_visible = true;
};