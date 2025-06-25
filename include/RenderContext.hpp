#pragma once
#include "glm/glm.hpp"
#include "components/Transform.hpp"

struct RenderContext
{
    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;
    Transform camTransform;
    float FOV;
    float aspect;
    float nearPlane;
    float farPlane;    

    static RenderContext& Instance()
    {
        static RenderContext instance;
        return instance;
    }

    void ResetProjMatrix()
    {
        projMatrix = glm::perspective(glm::radians(FOV), aspect, nearPlane, farPlane);
    }

    void ResetViewMatrix()
    {
        camTransform.ResetMatrixTransform();
        viewMatrix = glm::inverse(camTransform.matrixTransform);
    }

    private:
        RenderContext() = default;
        RenderContext(const RenderContext&) = delete;
        RenderContext& operator=(const RenderContext&) = delete;
};