#pragma once
#include <vector>
#include <glm/vec3.hpp>

struct Model
{
    std::vector<glm::vec3> vertices;
    std::vector<int> faceIndices;
};