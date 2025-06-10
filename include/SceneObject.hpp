#pragma once

#include "Model.hpp"
#include "Transform.hpp"
#include "string.h"

class SceneObject
{
    public:
        SceneObject();
        SceneObject(float3 position, float3 rotation, Model model, std::string name, Transform* parent = nullptr);
        ~SceneObject();
        Transform& GetTransform();
        Model& GetModel();
        std::string GetName() const;

        void SetTransform(Transform transform);
        void SetTransform(float3 position, float3 rotation, Transform* parent = nullptr);
        void SetModel(Model model);
        void SetName(std::string name);
    private:
        Transform m_transform;
        Model m_model;

        std::string m_name;
        int m_id;
};
