#pragma once

#include <glm/vec3.hpp>
#include "rendercomponents/Vertex.hpp"

//Lives on the CPU within the assets
struct MeshData
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};