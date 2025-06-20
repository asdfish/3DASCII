#include "renderHandling.hpp"
#include "Light.hpp"
#include <iostream>
#include <ranges>
#include <cmath>

void RenderScene(Scene& scene, const Camera& camera, const std::vector<Light>& lights, SceneSettings& settings, std::uint8_t* pixelBuffer, float* zbuffer)
{
    for(auto& model : scene.GetSceneObjects())
    {
        RenderModel(model.GetModel(), model.GetTransform().GetPos(), model.GetTransform().GetRot(), camera, lights, settings, pixelBuffer, zbuffer);
    }
}


void RenderModel(Model& model, float3 position, float3 rotation, const Camera& camera, const std::vector<Light>& lights, SceneSettings& settings, std::uint8_t* pixelBuffer, float* zbuffer)
{

    std::vector<float3> vertices = model.GetVerts(position, rotation);
    std::vector<int> faceIndices = model.GetFaceIndices();

    float3 modelColor = model.GetColor();

    for(auto itf = faceIndices.begin(); itf != faceIndices.end(); itf += 3)
    {
        //std::cout<<"Rendering triangle with indices: "<<*(itf  )<<" "<<*(itf+1)<<" "<<*(itf+2)<<"\n";

        float3 v1World = vertices[*(itf  )];
        float3 v2World = vertices[*(itf+1)];
        float3 v3World = vertices[*(itf+2)];

        float3 centroid = (v1World + v2World + v3World)/3;
        float3 normal = float3::cross(v2World-v1World, v3World-v1World);
        float normalMag = normal.abs();
        
        float3 faceColor = float3(0,0,0);

        //Flat Shading enabled
        if (settings.lightingMode == LightingMode::FLAT)
        {
            for (const auto& light : lights)
            {
                float3 vectorToLight = light.GetPosition() - centroid;
                float angleCos = float3::dot(vectorToLight, normal) / (vectorToLight.abs() * normalMag);
                angleCos = std::max(angleCos, 0.f);
                float sqDistance = (vectorToLight/settings.falloffCoeff).sqabs();
                sqDistance = std::max(sqDistance, 0.001f);

                float lightIntensity = light.GetIntensity()*settings.lightIntensityCoeff*angleCos/sqDistance;
                float3 lightColor = light.GetColor();
                float3 I_out = lightColor*lightIntensity;
                faceColor = faceColor + I_out * modelColor;
            }
            //Reinhard Tone Mapping Wooo
            faceColor = faceColor/(faceColor+float3(1,1,1)); 
        }
        float3 v1 = WorldToScreen(v1World, camera);
        float3 v2 = WorldToScreen(v2World, camera);
        float3 v3 = WorldToScreen(v3World, camera);

        //Backface culling
        if(EdgeFunction(v1, v2, v3)<0)
        {
            continue;
        }

        v1 = ScreenToPixelSpace(v1, camera);
        v2 = ScreenToPixelSpace(v2, camera);
        v3 = ScreenToPixelSpace(v3, camera);

        //std::cout<<"Compare with camera world dimensions: "<<camera.GetScreenWorldDimensions().x<<" "<<camera.GetScreenWorldDimensions().y<<"\n";
        if (v1.z <= 0 || v2.z <= 0 || v3.z <= 0)
        {
            continue; // Skip triangles that are behind the camera
        }
        
        float2 pixelDimensions = camera.GetPixelDimensions();

        if ((v1.x < 0 && v2.x < 0 && v3.x < 0) || // left
            (v1.x >  pixelDimensions.x && v2.x >  pixelDimensions.x && v3.x >  pixelDimensions.x) || // right
            (v1.y < 0 && v2.y < 0 && v3.y < 0) || // bottom
            (v1.y >  pixelDimensions.y && v2.y >  pixelDimensions.y && v3.y >  pixelDimensions.y))   // top
        {
            continue; // Entire triangle is outside view
        }
        
        //std::cout<<"v1: "<<v1.x<<" "<<v1.y<<" "<<v1.z<<" | v2: "<<v2.x<<" "<<v2.y<<" "<<v2.z<<" | v3: "<<v3.x<<" "<<v3.y<<" "<<v3.z<<"\n";

        //std::cout<<"color: "<<randColor.r<<" "<<randColor.g<<" "<<randColor.b<<"\n";

        int Xmin = static_cast<int>(std::floor(std::min(std::min(v1.x, v2.x), v3.x)));
        int Xmax = static_cast<int>(std::ceil(std::max(std::max(v1.x, v2.x), v3.x)));
        int Ymin = static_cast<int>(std::floor(std::min(std::min(v1.y, v2.y), v3.y)));
        int Ymax = static_cast<int>(std::ceil(std::max(std::max(v1.y, v2.y), v3.y)));

        Xmin = std::max(0, Xmin);
        Ymin = std::max(0, Ymin);
        Xmax = std::min(static_cast<int>(pixelDimensions.x), Xmax);
        Ymax = std::min(static_cast<int>(pixelDimensions.y), Ymax);


        for (int x = Xmin; x<Xmax; x++)
        {
            for (int y = Ymin; y<Ymax; y++)
            {
                auto bary = GetBarycentricCoords(float2(x+0.5,y+0.5), float2(v1.x, v1.y), float2(v2.x, v2.y), float2(v3.x, v3.y));
                if (bary)
                {
                    int screenWidth = static_cast<int>(pixelDimensions.x);
                    float z = 1/ PerspBarycentricInterp(*bary, 1/v1.z, 1/v2.z, 1/v3.z);
                    //std::cout<<"Interp z-value of pixel: "<<temp<<", zbuffer value: "<<zbuffer[y*screenWidth + x]<<", rewrite?: "<<(temp < zbuffer[y*screenWidth + x])<<"\n";
                    if (z < zbuffer[y*screenWidth + x])
                    {
                        //Flat shading
                        if (settings.lightingMode == LightingMode::FLAT)
                        {
                            //TEMP - all tris are white
                            pixelBuffer[4*(y*screenWidth + x) + 0] = static_cast<std::uint8_t>(faceColor.r * 255.f);
                            pixelBuffer[4*(y*screenWidth + x) + 1] = static_cast<std::uint8_t>(faceColor.g * 255.f);
                            pixelBuffer[4*(y*screenWidth + x) + 2] = static_cast<std::uint8_t>(faceColor.b * 255.f);
                            pixelBuffer[4*(y*screenWidth + x) + 3] = static_cast<std::uint8_t>(255.f);  //Opaque
                        }
                        if (settings.lightingMode == LightingMode::LAMBERTIAN)
                        {
                            float3 pixelColor = float3(0,0,0);
                            float2 screenSpacePos = PixelToScreenSpace(float2(x,y), camera);
                            float3 worldPos = ScreenToWorld(float3(screenSpacePos.x, screenSpacePos.y, z), camera);
                            for (const auto& light : lights)
                            {
                                float3 vectorToLight = light.GetPosition() - worldPos;
                                float angleCos = float3::dot(vectorToLight, normal) / (vectorToLight.abs() * normalMag);
                                angleCos = std::max(angleCos, 0.f);
                                float sqDistance = (vectorToLight/settings.falloffCoeff).sqabs();
                                sqDistance = std::max(sqDistance, 0.001f);

                                float lightIntensity = light.GetIntensity()*settings.lightIntensityCoeff*angleCos/sqDistance;
                                float3 lightColor = light.GetColor();
                                float3 I_out = lightColor*lightIntensity;
                                pixelColor = pixelColor + I_out * modelColor;
                            }
                            //Reinhard Tone Mapping Wooo
                            pixelColor = pixelColor/(pixelColor+float3(1,1,1)); 
                            //TEMP - all tris are white
                            pixelBuffer[4*(y*screenWidth + x) + 0] = static_cast<std::uint8_t>(pixelColor.r * 255.f);
                            pixelBuffer[4*(y*screenWidth + x) + 1] = static_cast<std::uint8_t>(pixelColor.g * 255.f);
                            pixelBuffer[4*(y*screenWidth + x) + 2] = static_cast<std::uint8_t>(pixelColor.b * 255.f);
                            pixelBuffer[4*(y*screenWidth + x) + 3] = static_cast<std::uint8_t>(255.f);  //Opaque
                        }
                        zbuffer[y*screenWidth + x] = z;
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
    float3 c = camera.GetTransform().GetPos();      //3D position of camera
    float3 theta = camera.GetTransform().GetRot();  //Euler Angles of camera
    float3 e = float3(0,0,camera.GetFocalLength()); //position of plane relative to camera (camera's rot, etc)

    float3 d;                                       //Point's position wrt the camera's transform
    float3 b;                                       //Projected coordinates of the point

    float x = a.x - c.x;                            //a.x - c.x
    float y = a.y - c.y;                            //a.y - c.y
    float z = a.z - c.z;                            //a.z - c.z

    float cosx = cos(theta.x), sinx = sin(theta.x);
    float cosy = cos(theta.y), siny = sin(theta.y);
    float cosz = cos(theta.z), sinz = sin(theta.z);

    d.x = cosy * (sinz*y + cosz*x) - siny*z;
    d.y = sinx * (cosy*z + siny * (sinz*y + cosz*x)) + cosx * (cosz * y - sinz * x);
    d.z = cosx * (cosy*z + siny * (sinz*y + cosz*x)) - sinx * (cosz * y - sinz * x);

    if (std::abs(d.z) < 1e-4f)
        d.z = (d.z >= 0 ? 1e-4f : -1e-4f); 

    b.x = (e.z * d.x)/d.z + e.x;
    b.y = (e.z * d.y)/d.z + e.y;
    b.z = d.z;

    return b;

}

float3 ScreenToWorld
(
    float3 b,               //Projected coordinates of the point
    const Camera& camera    //Transform of the camera
)
{
    /*
    Let cosθ_α be Cα and sinθ_α be Sα
    The Inverse matrices are: 
            ┌           ┐
            │ 1   0   0 │
    R-1_x = │ 0   Cx -Sx│
            │ 0   Sx  Cx│
            └           ┘

            ┌           ┐
            │ Cy  0   Sy│
    R-1_y = │ 0   1   0 │
            │-Sy  0   Cy│
            └           ┘

            ┌           ┐
            │ Cz -Sz  0 │
    R-1_z = │ Sz  Cz  0 │
            │ 0   0   1 │
            └           ┘
    
    The combined rotation matrix is

    R-1   = R-1_z * R-1_y * R-1_x
            ┌                                        ┐
            │ CzCy   -SzCx + CzSySx    SzSx + CzSyCx │
          = │ SzCy    CzCx + SzSySx   -CzSx + SzSyCx │
            │ -Sy         CySx              Cx       │
            └                                        ┘

    a = (R-1 * d) + c

    */

    float3 c = camera.GetTransform().GetPos();      //3D position of camera
    float3 theta = camera.GetTransform().GetRot();  //Euler Angles of camera
    float3 e = float3(0,0,camera.GetFocalLength()); //position of plane relative to camera (camera's rot, etc)

    float3 d;                                       //Point's position wrt the camera's transform
    float3 a;                                       //Projected coordinates of the point

    float cosx = cos(theta.x), sinx = sin(theta.x);
    float cosy = cos(theta.y), siny = sin(theta.y);
    float cosz = cos(theta.z), sinz = sin(theta.z);

    
    d.x = (b.x - e.x) * b.z / e.z;
    d.y = (b.y - e.y) * b.z / e.z;
    d.z = b.z;

    if (std::abs(d.z) < 1e-4f)
        d.z = (d.z >= 0 ? 1e-4f : -1e-4f); 

    a.x = (cosz*cosy) * d.x +
          (-sinz*cosx + cosz*siny*sinx) * d.y +
          (sinz*sinx + cosz*siny*cosx) * d.z + c.x;

    a.y = (sinz*cosy) * d.x + 
          (cosz*cosx + sinz*siny*sinx) * d.y + 
          (-cosz*sinx + sinz*siny*cosx) * d.z + c.y;

    a.z = (-siny) * d.x + 
          (cosy*sinx) * d.y + 
          (cosx) * d.z + c.z;
    
    return a;
}

float3 ScreenToPixelSpace(float3 screenCoords, const Camera& camera)
{
    float2 pixelDimensions = camera.GetPixelDimensions();
    float2 worldDimensions = camera.GetScreenWorldDimensions();

    float3 newCoords;
    newCoords.x = (screenCoords.x + worldDimensions.x/2) * (pixelDimensions.x/worldDimensions.x);
    newCoords.y = (screenCoords.y + worldDimensions.y/2) * (pixelDimensions.y/worldDimensions.y);
    newCoords.z = screenCoords.z; //Z-coordinate remains unchanged
    return newCoords;
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
    return baryCoords.x*w1 + baryCoords.y*w2 + baryCoords.z*w3;
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