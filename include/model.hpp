#include "image.hpp"
#include <vector>

#pragma once

class Transform
{
    
};



//Using for anything with a position, like vertices of a model, etc.
class Model
{
    public:
        Model(const std::vector<float3>& verts, const std::vector<int>& faceIndices);
        ~Model();

        const std::vector<float3>& GetVerts();
        const std::vector<float3>& GetFaceIndices();
    private:
        std::vector<float3> m_verts;
        std::vector<int> m_faceIndices;
};