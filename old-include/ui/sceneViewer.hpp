#pragma once

#include <optional>

#include "Scene.hpp"
#include "SceneObject.hpp"
#include "Light.hpp"
#include "camera.hpp"

class SceneViewer
{
    public:
        void Draw(Scene& scene, SceneObject*& selectedObject, std::vector<Light>& lights, Light*& selectedLight, Camera& camera, Camera*& selectedCamera, std::optional<Light>& copiedLight, std::optional<SceneObject>& copiedObject);
        void SetVisible(bool visible) { m_visible = visible; }
        bool GetVisible() const { return m_visible; }
    private:
        bool m_visible = true;
};