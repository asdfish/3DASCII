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

float float3::dot(float3 a, float3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float2::float2()
: x(0), y(0)
{
}

float2::float2(float x, float y)
: x(x), y(y)
{
}

float2::~float2()
{
}

float float2::dot(float2 a, float2 b)
{
    return a.x * b.x + a.y * b.y;
}
