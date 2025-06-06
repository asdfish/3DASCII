#include <iostream>
#include <vector>

#include "image.hpp"

int main() {
    const int height = 50;
    const int width = 50;
    Image newBmp(50, 50);
    for (int y = 0; y<height; y++)
    {
        for(int x = 0; x<width; x++)
        {
            newBmp.SetColor(float3((float)x/ (float)width, 1.0f - (float)x/ (float)width, (float)y/ (float)height), x, y);
        }
    }
    newBmp.Export("image.bmp");
} 

