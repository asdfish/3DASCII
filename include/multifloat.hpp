#pragma once

class float3
{
    union
    {
        struct {float x; float y; float z;};
        struct {float r; float g; float b;};
    };

    float3();
    float3(float x, float y, float z);
    ~float3();

    static float dot(float3 a, float3 b);

};

class float2
{
    public:
        float2();
        float2(float x, float y);
        ~float2();
        static float dot(float2 a, float2 b);

        float x,y;
};
