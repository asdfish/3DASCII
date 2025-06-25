#pragma once
#include "ecss-templates/system.hpp"
#include "managers.hpp"
#include "glad/gl.h"
#include "glm/glm.hpp"
#include "glm/gtx/projection.hpp"

class RenderSystem : public System
{
public:
    void Update();
};