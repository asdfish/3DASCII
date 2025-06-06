#include "image.hpp"
#include <vector>

#pragma once


class SceneObject
{
    public:
        SceneObject(float3 position, float3 rotation, const Transform* parent = nullptr, Model model);
        ~SceneObject();
        const Transform& GetTransform();
        const Model& GetModel();

        void SetTransform(Transform transform);
        void SetTransform(float3 position, float3 rotation, const Transform* parent = nullptr);
        void SetModel(Model model);
    private:
        Transform transform;
        Model model;
};
//contains position, rotation details of a sceneobject
class Transform
{
    public:
        Transform(float3 pos = float3(), float3 rot = float3(), Transform* parent = nullptr);
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