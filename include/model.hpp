#pragma once

#include "float3.hpp"

//Contains mesh details of individual meshes
class Model
{
    public:
        Model();
        Model(const std::vector<float3>& verts, const std::vector<int>& faceIndices);
        ~Model();

        const std::vector<float3>& GetVerts() const;
        const std::vector<int>& GetFaceIndices() const;

        float3 GetPivotFromVerts();
        
        void AddVert(float3 vert);
        void AddFace(int index1, int index2, int index3);
        void AddFaceIndex(int index);
    private:
        std::vector<float3> m_verts;
        std::vector<int> m_faceIndices;
};

