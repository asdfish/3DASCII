#include "renderHandling.hpp"
#include <iostream>

void RenderScene(const Scene& scene, const Camera& camera, std::uint8_t* pixelBuffer, float* zbuffer)
{
    for(const auto& model : scene.GetObjects())
    {
        RenderModel(model.GetModel(), camera, pixelBuffer, zbuffer);
    }
}


void RenderModel(const Model& model, const Camera& camera, std::uint8_t* pixelBuffer, float* zbuffer)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    std::vector<float3> vertices = model.GetVerts();
    std::vector<int> faceIndices = model.GetFaceIndices();

    for(auto itf = faceIndices.begin(); itf != faceIndices.end(); itf += 3)
    {
        //std::cout<<"Rendering triangle with indices: "<<*(itf  )<<" "<<*(itf+1)<<" "<<*(itf+2)<<"\n";

        float3 v1 = WorldToScreen(vertices[*(itf  )], camera);
        float3 v2 = WorldToScreen(vertices[*(itf+1)], camera);
        float3 v3 = WorldToScreen(vertices[*(itf+2)], camera);

        
        //std::cout<<"Compare with camera world dimensions: "<<camera.GetScreenWorldDimensions().x<<" "<<camera.GetScreenWorldDimensions().y<<"\n";
        if (v1.z <= 0 || v2.z <= 0 || v3.z <= 0)
        {
            continue; // Skip triangles that are behind the camera
        }

        float2 wd = camera.GetScreenWorldDimensions();

        float halfW = wd.x/2.0f;
        float halfH = wd.y/2.0f;
        
        if ((v1.x < -halfW && v2.x < -halfW && v3.x < -halfW) || // left
            (v1.x >  halfW && v2.x >  halfW && v3.x >  halfW) || // right
            (v1.y < -halfH && v2.y < -halfH && v3.y < -halfH) || // bottom
            (v1.y >  halfH && v2.y >  halfH && v3.y >  halfH))   // top
        {
            continue; // Entire triangle is outside view
        }
        
        //std::cout<<"v1: "<<v1.x<<" "<<v1.y<<" "<<v1.z<<" | v2: "<<v2.x<<" "<<v2.y<<" "<<v2.z<<" | v3: "<<v3.x<<" "<<v3.y<<" "<<v3.z<<"\n";

        float3 randColor = float3(dist(gen), dist(gen), dist(gen));

        //std::cout<<"color: "<<randColor.r<<" "<<randColor.g<<" "<<randColor.b<<"\n";

        float2 pixelDimensions = camera.GetPixelDimensions();

        //TODO: Use Bounding Box of Tri for faster rasterization

        for (int x = 0; x<pixelDimensions.x; ++x)
        {
            for (int y = 0; y<pixelDimensions.y; ++y)
            {
                PixelToScreenSpace(float2(x+0.5f,y+0.5f), camera);

                auto bary = GetBarycentricCoords(PixelToScreenSpace(float2(x+0.5f,y+0.5f), camera), float2(v1.x, v1.y), float2(v2.x, v2.y), float2(v3.x, v3.y));
                if (bary)
                {
                    int screenWidth = static_cast<int>(pixelDimensions.x);
                    
                    float temp = 1/ PerspBarycentricInterp(*bary, 1/v1.z, 1/v2.z, 1/v3.z);
                    //std::cout<<"Interp z-value of pixel: "<<temp<<", zbuffer value: "<<zbuffer[y*screenWidth + x]<<", rewrite?: "<<(temp < zbuffer[y*screenWidth + x])<<"\n";
                    if (temp < zbuffer[y*screenWidth + x])
                    {
                        pixelBuffer[4*(y*screenWidth + x)    ] = static_cast<std::uint8_t>(randColor.r * 255.f);
                        pixelBuffer[4*(y*screenWidth + x) + 1] = static_cast<std::uint8_t>(randColor.g * 255.f);
                        pixelBuffer[4*(y*screenWidth + x) + 2] = static_cast<std::uint8_t>(randColor.b * 255.f);
                        pixelBuffer[4*(y*screenWidth + x) + 3] = static_cast<std::uint8_t>(255.f);  //Opaque
                        zbuffer[y*screenWidth + x] = temp;

                    }
                }
            }
        }
    }
}


//Projects a world point to a screen point w.r.t. the camera.
//For the math, see https://en.wikipedia.org/wiki/3D_projection#Mathematical_formula
float3 WorldToScreen
(
    float3 a,               //Position of the point
    const Camera& camera    //Transform of the camera
)
{
    float3 c = camera.GetTransform().GetPos();     //3D position of camera
    float3 theta = camera.GetTransform().GetRot(); //Euler Angles of camera
    float3 e = float3(0,0,camera.GetFocalLength());             //position of plane relative to camera (camera's rot, etc)

    float3 d;                                      //Point's position wrt the camera's transform
    float3 b;                                      //Projected coordinates of the point

    float x = a.x - c.x;                           //a.x - c.x
    float y = a.y - c.y;                           //a.y - c.y
    float z = a.z - c.z;                           //a.z - c.z

    float cosx = cos(theta.x), sinx = sin(theta.x);
    float cosy = cos(theta.y), siny = sin(theta.y);
    float cosz = cos(theta.z), sinz = sin(theta.z);

    d.x = cosy * (sinz*y + cosz*x) - siny*z;
    d.y = sinx * (cosy*z + siny * (sinz*y + cosz*x)) + cosx * (cosz * y - sinz * x);
    d.z = cosx * (cosy*z + siny * (sinz*y + cosz*x)) - sinx * (cosz * y - sinz * x);

    b.x = (e.z * d.x)/d.z + e.x;
    b.y = (e.z * d.y)/d.z + e.y;
    b.z = d.z;

    return b;

}



float2 PixelToScreenSpace(float2 pixelCoords, const Camera& camera)
{
    float2 newCoords;
    float2 pixelDimensions = camera.GetPixelDimensions();
    float2 worldDimensions = camera.GetScreenWorldDimensions();

    newCoords.x = (pixelCoords.x - pixelDimensions.x/2) * (worldDimensions.x/pixelDimensions.x);
    newCoords.y = (pixelCoords.y - pixelDimensions.y/2) * (worldDimensions.y/pixelDimensions.y);
    return newCoords;
}

//Perspective-corrected Barycentric Interpolation
float PerspBarycentricInterp(float3 baryCoords, float w1, float w2, float w3)
{
    return ((baryCoords.x*w1 + baryCoords.y*w2 + baryCoords.z*w3) / (baryCoords.x/w1 + baryCoords.y/w2 + baryCoords.z/w3));
}

std::optional<float3> GetBarycentricCoords(float2 point, float2 v1, float2 v2, float2 v3)
{

    float3 w;
    float area = EdgeFunction(v1, v2, v3);
    
    w.x = EdgeFunction(point, v1, v2);
    w.y = EdgeFunction(point, v2, v3);
    w.z = EdgeFunction(point, v3, v1);

    const float epsilon = 1e-5f;
    if (w.x >= -epsilon && w.y >= -epsilon && w.z >= -epsilon)
    {
        //std::cout<<"w pre-scale: "<<w.x<<" "<<w.y<<" "<<w.z<<"\n";
        //std::cout<<"Area: "<<area<<"\n";
        w.x /= area, w.y /= area, w.z /= area;
        //std::cout<<"w post-scale: "<<w.x<<" "<<w.y<<" "<<w.z<<"\n";
        return w;
    }
    return std::nullopt;
}


float EdgeFunction(float2 point, float2 v1, float2 v2)
{
    return ((point.x - v1.x) * (v2.y-v1.y) - (point.y - v1.y) * (v2.x - v1.x));
}