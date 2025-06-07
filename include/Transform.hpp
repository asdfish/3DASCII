#pragma once

#include "multifloat.hpp"

//contains position, rotation details of a sceneobject
class Transform
{
    public:
        Transform(float3 pos = float3(), float3 rot = float3(), const Transform* parent = nullptr);
        ~Transform();

        const float3& GetPos() const;
        const float3& GetRot() const;
        const Transform* GetParent() const;

        void SetPos(const float3& newPos);
        void SetRot(const float3& newRot);
        void SetParent(const Transform* parent);

    private:
        float3 m_pos;
        float3 m_rot;
        const Transform* m_parent;
};