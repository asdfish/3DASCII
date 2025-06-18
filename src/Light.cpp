#include "Light.hpp"

Light::Light(float3 pos, float intensity)
:m_position(pos), m_intensity(intensity), m_color(1,1,1)
{
    m_name = "Light";
    m_id = uuid::generate_uuid_v4();
}

Light::Light(float3 pos, float intensity, std::string name)
:m_position(pos), m_intensity(intensity), m_name(name)
{
    m_id = uuid::generate_uuid_v4();
}

Light::Light(float3 pos, float intensity, std::string name, std::string id, float3 color)
:m_position(pos), m_intensity(intensity), m_name(name), m_id(id), m_color(color)
{
}

Light::~Light()
{
}

float3 Light::GetPosition() const
{
    return m_position;
}

float Light::GetIntensity() const
{
    return m_intensity;
}

std::string Light::GetName() const
{
    return m_name;
}

std::string Light::GetID() const
{
    return m_id;
}

const float3& Light::GetColor() const
{
    return m_color;
}

void Light::SetPosition(const float3& pos)
{
    m_position = pos;
}
void Light::SetIntensity(float intensity)
{
    m_intensity = intensity;
}

void Light::SetName(std::string name)
{
    m_name = name;
}

void Light::SetColor(const float3 &color)
{
    m_color = color;
}
