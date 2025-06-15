#pragma once

#include "Scene.hpp"
#include "SceneObject.hpp"
#include "Light.hpp"
#include "camera.hpp"

class InspectorMenu
{
    public:
        void Draw(SceneObject*& selectedObject, Light*& selectedLight, Camera*& selectedCamera);
        void SetVisible(bool visible) { m_visible = visible; }
        bool GetVisible() const { return m_visible; }
    private:
        bool m_visible = true;
};