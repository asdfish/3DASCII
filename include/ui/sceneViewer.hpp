#pragma once

#include "Scene.hpp"
#include "SceneObject.hpp"

class SceneViewer
{
    public:
        void Draw(Scene& scene, SceneObject*& selectedObject);
        void SetVisible(bool visible) { m_visible = visible; }
        bool GetVisible() const { return m_visible; }
    private:
        bool m_visible = true;
};