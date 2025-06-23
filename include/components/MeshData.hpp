#pragma once

#include <glm/vec3.hpp>

//Lives on the CPU within the assets
struct MeshData
{
    std::vector<glm::vec4> vertices;
    std::vector<unsigned int> indices;
};