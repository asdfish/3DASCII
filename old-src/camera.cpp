#include "camera.hpp"
#include <cmath>

Camera::Camera(float3 pos, float3 rot, float FOV, float focalLength, float pixelWidth, float pixelHeight)
: m_transform(pos, rot, nullptr), m_FOV(FOV), m_focalLength(focalLength), m_dimensions(pixelWidth, pixelHeight)
{
}

Camera::~Camera()
{
}

const Transform& Camera::GetTransform() const
{
    return m_transform;
}

Transform &Camera::GetModifiableTransform()
{
    return m_transform;
}

float Camera::GetFOV() const
{
    return m_FOV;
}

float Camera::GetFocalLength() const
{
    return m_focalLength;
}

float2 Camera::GetPixelDimensions() const
{
    return m_dimensions;
}

void Camera::SetFOV(float newFOV)
{
    m_FOV = newFOV;
}

void Camera::SetPixelDimensions(float pixelWidth, float pixelHeight)
{
    m_dimensions = float2(pixelWidth, pixelHeight);
}

float2 Camera::GetScreenWorldDimensions() const
{
    float temp = 2*tan((m_FOV/2)*(M_PI/180.f))*m_focalLength;
    return float2(
        temp * (m_dimensions.x/m_dimensions.y),
        temp
    );
}

void Camera::SetFocalLength(float newFocalLength)
{
    m_focalLength = newFocalLength;
}


