#include "ui.hpp"
#include <memory>

void DisplayUI(Scene& scene)
{
    static SceneObject* selectedObject = nullptr; 

    Toolbar toolbar;
    AssetBrowser assetBrowser;
    InspectorMenu inspectorMenu;
    SceneViewer sceneViewer;

    // Draw the Toolbar (or main menu bar)
    toolbar.Draw(scene);

    // Draw the Asset Browser
    assetBrowser.Draw(scene, selectedObject);

    // Draw the Scene Viewer
    sceneViewer.Draw(scene, selectedObject);

    // Draw the Inspector Menu
    inspectorMenu.Draw(selectedObject);
}