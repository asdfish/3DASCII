#pragma once

#include <iostream>
#include <vector>
#include "multifloat.hpp"

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

