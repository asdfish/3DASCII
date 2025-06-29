#pragma once
#include "ecss-templates/system.hpp"
#include "managers.hpp"
#include "glad/gl.h"
#include "glm/glm.hpp"
#include "glm/gtx/projection.hpp"

class ASCIIRenderSystem : public System
{
public:
    void Init();
    void Update();
};