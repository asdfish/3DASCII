#pragma once
#include <cmath>

class float3
{
    public:
        union
        {
            struct {float x; float y; float z;};
            struct {float r; float g; float b;};
        };

        float3();
        float3(float x, float y, float z);
        ~float3();

        float sqabs() const;
        float abs() const;

        static float dot(const float3& a, const float3& b);
        static float3 cross(const float3& a, const float3& b);
};

inline float3 operator+(const float3& lhs, const float3& rhs)
{
    return float3(
        lhs.x + rhs.x,
        lhs.y + rhs.y,
        lhs.z + rhs.z
    );
}

inline float3 operator-(const float3& lhs, const float3& rhs)
{
    return float3(
        lhs.x - rhs.x,
        lhs.y - rhs.y,
        lhs.z - rhs.z
    );
}

inline float3 operator*(const float3& lhs, const float3& rhs)
{
        return float3(
        lhs.x * rhs.x,
        lhs.y * rhs.y,
        lhs.z * rhs.z
    );
}

inline float3 operator/(const float3& lhs, const float3& rhs)
{
        return float3(
        lhs.x / rhs.x,
        lhs.y / rhs.y,
        lhs.z / rhs.z
    );
}

inline float3 operator*(const float3& lhs, float rhs)
{
        return float3(
        lhs.x * rhs,
        lhs.y * rhs,
        lhs.z * rhs
    );
}

inline float3 operator/(const float3& lhs, float rhs)
{
        return float3(
        lhs.x / rhs,
        lhs.y / rhs,
        lhs.z / rhs
    );
}

class float2
{
    public:
        float2();
        float2(float x, float y);
        float2(float3 toCompress);
        ~float2();

        float abs() const;

        static float dot(float2 a, float2 b);

        float x,y;
};

inline float2 operator+(const float2& lhs, const float2& rhs)
{
    return float2(
        lhs.x + rhs.x,
        lhs.y + rhs.y
    );
}

inline float2 operator-(const float2& lhs, const float2& rhs)
{
    return float2(
        lhs.x - rhs.x,
        lhs.y - rhs.y
    );
}

inline float2 operator*(const float2& lhs, float rhs)
{
        return float2(
        lhs.x * rhs,
        lhs.y * rhs
    );
}

inline float2 operator/(const float2& lhs, float rhs)
{
        return float2(
        lhs.x / rhs,
        lhs.y / rhs
    );
}