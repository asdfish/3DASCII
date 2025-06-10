#pragma once

#include "multifloat.hpp"

//contains position, rotation details of a sceneobject
class Transform
{
    public:
        Transform(float3 pos = float3(), float3 rot = float3(), Transform* parent = nullptr);
        ~Transform();

        float3 GetPos() const;
        float3 GetRot() const;
        Transform* GetParent();

        void SetPos(const float3& newPos);
        void SetRot(const float3& newRot);
        void SetParent(Transform* parent);

    private:
        float3 m_pos;
        float3 m_rot;
        Transform* m_parent;
};