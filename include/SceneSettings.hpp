#pragma once

enum class LightingMode {FLAT, LAMBERTIAN};
enum class RenderMode {NORMAL, ASCIIBASIC};

struct SceneSettings
{
    //0 for flat, 1 for lambertian
    LightingMode lightingMode = LightingMode::FLAT;
    RenderMode renderMode = RenderMode::NORMAL;
    float lightIntensityCoeff = 100.f;
    float falloffCoeff = 1.f;
};

