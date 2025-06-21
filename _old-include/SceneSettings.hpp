#pragma once

enum class LightingMode {FLAT, LAMBERTIAN};
enum class RenderMode {NORMAL, ASCIIBASIC};

struct SceneSettings
{
    /* Lighting and rendering */
    LightingMode lightingMode = LightingMode::FLAT;
    RenderMode renderMode = RenderMode::NORMAL;
    float lightIntensityCoeff = 100.f;
    float falloffCoeff = 1.f;

    /* Camera Movement */
    float scrollVel = 2.f;
    float panVel = 0.2f;
    float rotVel = 0.001f;
    float FOVChangeSpeed = 6.f;
    float motionVel = 2.f;
};

