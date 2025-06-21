#pragma once

#include <glm/vec2.hpp>

struct Camera
{
    float FOV;
    float focalLength;
    glm::vec2 screenDimensions;
};