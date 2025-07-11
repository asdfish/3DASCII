#include "image.hpp"
#include <vector>
#include <iostream>
#include <fstream>

Image::Image(int width, int height)
: m_width(width), m_height(height), m_colors(std::vector<float3>(width*height))
{
}

Image::~Image()
{
}

float3 Image::GetColor(int x, int y) const
{
    return m_colors[y*m_width + x];
}

void Image::SetColor(const float3 &color, int x, int y)
{
    m_colors[y*m_width + x].r = color.r;
    m_colors[y*m_width + x].g = color.g;
    m_colors[y*m_width + x].b = color.b;
}

void Image::Export(const char *path) const
{
    std::ofstream f;
    f.open(path, std::ios::binary);

    if (!f.is_open())
    {
        std::cout<<"File cannot be opened\n";
        return;
    }

    unsigned char bmpPad[3] = {0,0,0};
    const int paddingAmount = ((4 - (m_width * 3) % 4) % 4);

    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;
    const int fileSize = fileHeaderSize + informationHeaderSize + m_width*m_height*3 + paddingAmount*m_height;


    unsigned char fileHeader[fileHeaderSize];

    //File Type
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    //File Size
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;
    //Reserved 1 (unused)
    fileHeader[5] = 0;
    fileHeader[6] = 0;
    //Reserved 2 (unused)
    fileHeader[7] = 0;
    fileHeader[8] = 0;
    //Pixel Data Offset
    fileHeader[10] = fileHeaderSize + informationHeaderSize;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;

    unsigned char informationHeader[informationHeaderSize];
    //Header Size
    informationHeader[0] = informationHeaderSize;
    informationHeader[1] = 0;
    informationHeader[2] = 0;
    informationHeader[3] = 0;
    //Image Width
    informationHeader[4] = m_width;
    informationHeader[5] = m_width >> 8;
    informationHeader[6] = m_width >> 16;
    informationHeader[7] = m_width >> 24;
    //Image Height
    informationHeader[8] = m_height;
    informationHeader[9] = m_height >> 8;
    informationHeader[10] = m_height >> 16;
    informationHeader[11] = m_height >> 24;
    //Planes
    informationHeader[12] = 1;
    informationHeader[13] = 0;
    //Bits per pixel (RGB)
    informationHeader[14] = 24;
    informationHeader[15] = 0;
    //Compression (no compression please for the love of god)
    informationHeader[16] = 0;
    informationHeader[17] = 0;
    informationHeader[18] = 0;
    informationHeader[19] = 0;
    //Image size change or sth (also 0 cause no compression)
    informationHeader[20] = 0;
    informationHeader[21] = 0;
    informationHeader[22] = 0;
    informationHeader[23] = 0;
    //X Pixels per meter (unspecified)
    informationHeader[24] = 0;
    informationHeader[25] = 0;
    informationHeader[26] = 0;
    informationHeader[27] = 0;
    //Y Pixels per meter (unspecified)
    informationHeader[28] = 0;
    informationHeader[29] = 0;
    informationHeader[30] = 0;
    informationHeader[31] = 0;
    //total colors (no color palette)
    informationHeader[32] = 0;
    informationHeader[33] = 0;
    informationHeader[34] = 0;
    informationHeader[35] = 0;
    //important colors (IGNORED HA)
    informationHeader[36] = 0;
    informationHeader[37] = 0;
    informationHeader[38] = 0;
    informationHeader[39] = 0;

    f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
    f.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

    for (int y = 0; y<m_height;y++)
    {
        for (int x = 0; x<m_width;x++)
        {
            unsigned char r = static_cast<unsigned char>(GetColor(x,y).r * 255.0f);
            unsigned char g = static_cast<unsigned char>(GetColor(x,y).g * 255.0f);
            unsigned char b = static_cast<unsigned char>(GetColor(x,y).b * 255.0f);

            unsigned char color[] = {b, g, r};
            f.write(reinterpret_cast<char*>(color), 3);
        }
        f.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
    }
    f.close();

    std::cout<<"File created successfully \n";
}
