#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "modelHandling.hpp"
#include "renderHandling.hpp"
#include "camera.hpp"

#define wHeight 500
#define wWidth 500

int main() {

    //Setup

    Camera camera = Camera(float3(), float3(), 30, 1, wHeight, wWidth);
    Scene scene;
    objImporter("test.obj", scene);
    scene.GetObjectList();
    scene.GetObjectData();

    float zbuffer[wHeight*wWidth];
    std::uint8_t pixelBuffer[wHeight*wWidth*4];

    sf::Texture texture({wWidth, wHeight});
    sf::Image image;
    image.resize({wWidth, wHeight});

    sf::RenderWindow window(sf::VideoMode({wHeight, wWidth}), "ImGui + SFML = <3");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    //Loop

    sf::Clock deltaClock;
    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        //Update
        std::fill(pixelBuffer, pixelBuffer + (wWidth * wHeight * 4), 0);
        std::fill(zbuffer, zbuffer + (wWidth * wHeight), FLT_MAX);
        ImGui::SFML::Update(window, deltaClock.restart());

        RenderScene(scene, camera, pixelBuffer, zbuffer);

        for(unsigned int y = 0; y<wHeight; ++y)
        {
            for(unsigned int x = 0; x<wWidth; ++x)
            {
                int index = 4*(y*wWidth + x);
                sf::Color c(pixelBuffer[index + 0],
                            pixelBuffer[index + 1],
                            pixelBuffer[index + 2],
                            pixelBuffer[index + 3]);
                image.setPixel({x,y},c);
            }
            
        }
        texture.update(image);

        sf::Sprite sprite(texture);

        float3 cameraRot = camera.GetTransform().GetRot();
        float3 cameraPos = camera.GetTransform().GetPos();

        ImGui::Begin("Camera Controls");
        ImGui::SliderFloat("Camera Rotation - X", &(cameraRot.x) , 0.f, 2.f*M_PI);
        ImGui::SliderFloat("Camera Rotation - Y", &(cameraRot.y) , 0.f, 2.f*M_PI);
        ImGui::SliderFloat("Camera Rotation - Z", &(cameraRot.z) , 0.f, 2.f*M_PI);
        ImGui::SliderFloat("Camera Position - X", &(cameraPos.x) , -5.f, 5.f);
        ImGui::SliderFloat("Camera Position - Y", &(cameraPos.y) , -5.f, 5.f);
        ImGui::SliderFloat("Camera Position - Z", &(cameraPos.z) , -5.f, 5.f);
        ImGui::End();

        window.clear();
        window.draw(sprite);
        ImGui::SFML::Render(window);
        window.display();
        
        float3 prevRot = camera.GetTransform().GetRot();
        camera.GetModifiableTransform().SetRot(cameraRot);
        camera.GetModifiableTransform().SetPos(cameraPos);
    }

    ImGui::SFML::Shutdown();
}