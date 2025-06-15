#include "Light.hpp"

Light::Light(float3 pos, float intensity)
:m_position(pos), m_intensity(intensity)
{
    m_name = "Light";
    m_id = uuid::generate_uuid_v4();
}

Light::Light(float3 pos, float intensity, std::string name)
:m_position(pos), m_intensity(intensity), m_name(name)
{
    m_id = uuid::generate_uuid_v4();
}

Light::Light(float3 pos, float intensity, std::string name, std::string id)
:m_position(pos), m_intensity(intensity), m_name(name), m_id(id)
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
