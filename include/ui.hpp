#pragma once

#include "imgui.h"

#include "ui/assetBrowser.hpp"
#include "ui/inspectorMenu.hpp"
#include "ui/sceneViewer.hpp"
#include "ui/toolbar.hpp"
#include "ui/sceneSettingsMenu.hpp"


void DisplayUI(Scene& scene, std::vector<Light>& lights, Camera& camera, SceneSettings& settings);