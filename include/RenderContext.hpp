#pragma once
#include "glm/glm.hpp"

struct RenderContext
{
    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;
    glm::vec3 camPos;
};