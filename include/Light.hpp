#pragma once

#include "multifloat.hpp"
#include "uuid.hpp"

//PointLight for now
class Light
{
    public:
        Light(float3 pos, float intensity);
        Light(float3 pos, float intensity, std::string name);
        Light(float3 pos, float intensity, std::string name, std::string id, float3 color);
        ~Light();

        float3 GetPosition() const;
        float GetIntensity() const;
        std::string GetName() const;
        std::string GetID() const;
        const float3& GetColor() const;

        void SetPosition(const float3& pos);
        void SetIntensity(float intensity);
        void SetName(std::string name);
        void SetColor(const float3& color);
        void ChangeID();
    private:
        float3 m_position;
        float3 m_color;
        float m_intensity;
        std::string m_name;
        std::string m_id;
};