#include <iostream>
#include <vector>

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

class Image
{
    public :
        Image(int width, int height);
        ~Image();

        float3 GetColor(int x, int y) const;
        void SetColor(const float3& color, int x, int y);

        void Export(const char* path) const;

    private:
        int m_width;
        int m_height;
        std::vector<float3> m_colors;
};

