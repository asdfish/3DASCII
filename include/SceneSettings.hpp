#pragma once

enum class LightingMode {FLAT, LAMBERTIAN};

struct SceneSettings
{
    //0 for flat, 1 for lambertian
    LightingMode lightingMode = LightingMode::FLAT;
    float lightIntensityCoeff = 100.f;
    float falloffCoeff = 1.f;
};

