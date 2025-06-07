#pragma once

struct float3
{
    union
    {
        struct {float x; float y; float z;};
        struct {float r; float g; float b;};
    };

    float3();
    float3(float x, float y, float z);
    ~float3();




};

struct float2
{
    public:
        float2();
        float2(float x, float y);
        ~float2();
        int x,y;
};
