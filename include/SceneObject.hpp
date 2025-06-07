#pragma once

#include "Model.hpp"
#include "Transform.hpp"
#include "string.h"

class SceneObject
{
    public:
        SceneObject();
        SceneObject(float3 position, float3 rotation, Model model, std::string name, const Transform* parent = nullptr);
        ~SceneObject();
        const Transform& GetTransform() const;
        const Model& GetModel() const;
        std::string GetName() const;

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
