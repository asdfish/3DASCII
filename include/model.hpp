#include "image.hpp"
#include <vector>

#pragma once

class Scene
{
    public:
        Scene();
        ~Scene();

        void AddObject(const SceneObject& object);
        void GetObjectList();
    private:
        std::vector<SceneObject> m_objects;
};

class SceneObject
{
    public:
        SceneObject();
        SceneObject(float3 position, float3 rotation, Model model, std::string name, const Transform* parent = nullptr);
        ~SceneObject();
        const Transform& GetTransform();
        const Model& GetModel();
        std::string GetName();

        void SetTransform(Transform transform);
        void SetTransform(float3 position, float3 rotation, const Transform* parent = nullptr);
        void SetModel(Model model);
        void SetName(std::string name);
    private:
        Transform m_transform;
        Model m_model;

        std::string m_name;
        int m_id;
};

//contains position, rotation details of a sceneobject
class Transform
{
    public:
        Transform(float3 pos = float3(), float3 rot = float3(), const Transform* parent = nullptr);
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
        const Transform* m_parent;
};

//Contains mesh details of individual meshes
class Model
{
    public:
        Model();
        Model(const std::vector<float3>& verts, const std::vector<int>& faceIndices);
        ~Model();

        const std::vector<float3>& GetVerts();
        const std::vector<int>& GetFaceIndices();

        float3 GetPivotFromVerts();
        
        void AddVert(float3 vert);
        void AddFace(int index1, int index2, int index3);
        void AddFaceIndex(int index);
    private:
        std::vector<float3> m_verts;
        std::vector<int> m_faceIndices;
};

void objImporter(const char* path);