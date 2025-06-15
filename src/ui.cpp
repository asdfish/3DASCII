#include "ui.hpp"
#include <memory>

void DisplayUI(Scene& scene, std::vector<Light>& lights, Camera& camera, SceneSettings& settings)
{
    static SceneObject* selectedObject = nullptr; 
    static Light* selectedLight = nullptr;
    static Camera* selectedCamera = nullptr;

    Toolbar toolbar;
    AssetBrowser assetBrowser;
    InspectorMenu inspectorMenu;
    SceneViewer sceneViewer;
    SceneSettingsMenu sceneSettingsMenu;

    // Draw the Toolbar (or main menu bar)
    toolbar.Draw(scene, lights, camera, settings);

    // Draw the Asset Browser
    assetBrowser.Draw(scene, selectedObject);

    // Draw the Scene Viewer
    sceneViewer.Draw(scene, selectedObject, lights, selectedLight, camera, selectedCamera);

    // Draw the Inspector Menu
    inspectorMenu.Draw(selectedObject, selectedLight, selectedCamera);

    // Draw the Scene Settings Menu
    sceneSettingsMenu.Draw(settings);
}