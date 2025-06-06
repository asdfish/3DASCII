#include "image.hpp"
#include <vector>

#pragma once

//contains position, rotation details of a sceneobject
class Transform
{
    public:
        Transform(Transform* parent = nullptr, float3 rot = float3(), float3 pos = float3());
        ~Transform();

        const float3& GetPos();
        const float3& GetRot();
        const Transform* GetParent();

        void SetPos(const float3& newPos);
        void SetRot(const float3& newRot);
        void SetParent(const Transform* parent);

    private:
        float3 m_pos;
        float3 m_rot;
        Transform* m_parent;
};

//Contains mesh details of individual meshes
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