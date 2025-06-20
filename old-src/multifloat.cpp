#include "multifloat.hpp"


float3::float3()
    : x(0), y(0), z(0)
{
}

float3::float3(float x, float y, float z)
: x(x), y(y), z(z)
{
}

float3::~float3()
{
}

float float3::sqabs() const
{
    return powf(x, 2) + powf(y, 2) + powf(z, 2);
}

float float3::abs() const
{
    return sqrtf((powf(x, 2) + powf(y, 2) + powf(z, 2)));
}

float float3::dot(const float3& a, const float3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float3 float3::cross(const float3& a, const float3& b)
{
    return float3(
        (a.y * b.z) - (a.z * b.y),
        (a.x * b.z) - (a.z * b.x),
        (a.x * b.y) - (a.y * b.x)
    );
}


float2::float2()
: x(0), y(0)
{
}

float2::float2(float x, float y)
: x(x), y(y)
{
}

float2::float2(float3 toCompress)
: x(toCompress.x), y(toCompress.y)
{
}

float2::~float2()
{
}

float float2::dot(float2 a, float2 b)
{
    return a.x * b.x + a.y * b.y;
}
