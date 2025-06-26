#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform
{
    glm::vec3 position;
    glm::quat rotation;
    mutable glm::mat4 matrixTransform;

    Transform()
        : position(0.0f), rotation(), matrixTransform(1.0f)
    {
        ResetMatrixTransform();
    }

    Transform(glm::vec3 pos, glm::quat rot)
    : position(pos), rotation(rot)
    {
        ResetMatrixTransform();
    }

    void ResetMatrixTransform() const
    {
        glm::mat4 posMatrix = glm::translate(glm::mat4(1.f), position);
        glm::mat4 rotMatrix = glm::toMat4(rotation);

        matrixTransform = (rotMatrix* posMatrix);
    }

    glm::vec3 Right() const
    {
        return glm::vec3(matrixTransform[0]);
    }

    glm::vec3 Up() const
    {
        return glm::vec3(matrixTransform[1]);
    }

    glm::vec3 Forward() const
    {
        return -glm::vec3(matrixTransform[2]);
    }
};