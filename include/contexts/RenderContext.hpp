#pragma once
#include "glm/glm.hpp"
#include "components/Transform.hpp"

struct RenderContext
{
    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;
    Transform camTransform;

    float camPitch = 0.f;
    float camYaw = 0.f;

    float FOV;
    float aspect;
    float nearPlane;
    float farPlane;    

    float scrollVel = 2.f;
    float panVel = 0.2f;
    float rotVel = 0.001f;
    float FOVChangeSpeed = 6.f;
    float motionVel = 2.f;

    glm::vec4 lightPos = {0,0,0,1};
    glm::vec3 lightCol = {1, 1, 1};
    glm::vec3 objectCol = {1, 1, 1};
    float ambient = 0.2f;

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
        camTransform.rotation = glm::normalize(glm::angleAxis(camYaw, glm::vec3(0,1,0)) * glm::angleAxis(camPitch, glm::vec3(1,0,0)));
        glm::mat4 posMatrix = glm::translate(glm::mat4(1.f), camTransform.position);
        glm::mat4 rotMatrix = glm::toMat4(camTransform.rotation);
        viewMatrix = glm::inverse(posMatrix*rotMatrix);
        camTransform.ResetMatrixTransform();
    }

    private:
        RenderContext() = default;
        RenderContext(const RenderContext&) = delete;
        RenderContext& operator=(const RenderContext&) = delete;
};