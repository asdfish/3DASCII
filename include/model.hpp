#pragma once

#include "multifloat.hpp"
#include <vector>
#include <cmath>

//Contains mesh details of individual meshes
class Model
{
    public:
        Model();
        Model(const std::vector<float3>& verts, const std::vector<int>& faceIndices);
        ~Model();

        std::vector<float3> GetVerts(const float3& position, const float3& rotation) const;
        const std::vector<float3>& GetLocalVerts() const; //Use this to get the vertices in local space, or before the pivot is initialized
        const std::vector<int>& GetFaceIndices() const;

        float3 InitializePositionsFromPivot();
        
        void AddVert(float3 vert);
        void AddFace(int index1, int index2, int index3);
        void AddFaceIndex(int index);

    private:
        std::vector<float3> m_verts;
        std::vector<int> m_faceIndices;
};

