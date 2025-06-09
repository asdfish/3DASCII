#pragma once

#include "Transform.hpp"

class Camera
{
    public:
        Camera(float3 pos, float3 rot, float FOV, float focalLength, float pixelWidth, float pixelHeight);
        ~Camera();

        const Transform& GetTransform() const;
        Transform& GetModifiableTransform();
        float GetFOV() const;
        float GetFocalLength() const;
        float2 GetPixelDimensions() const;

        void SetFOV(float newFOV);
        void SetFocalLength(float newFocalLength);
        void SetPixelDimensions(float pixelWidth, float pixelHeight);

        float2 GetScreenWorldDimensions() const;
    private:
        Transform m_transform;
        float m_FOV;
        float m_focalLength;
        float2 m_dimensions;
};