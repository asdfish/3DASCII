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

#define wHeight 20
#define wWidth 20

int main() {

    //Setup

    Camera camera = Camera(float3(), float3(), 60, 1, wHeight, wWidth);
    Scene scene;
    objImporter("test.obj", scene);

    float zbuffer[wHeight*wWidth];
    std::uint8_t pixelBuffer[wHeight*wWidth*4];
    std::fill(pixelBuffer, pixelBuffer + (wWidth * wHeight * 4), 0);
    std::fill(zbuffer, zbuffer + (wWidth * wHeight), FLT_MAX);

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
        bool loadedToTexture = texture.loadFromImage(image);

        sf::Sprite sprite(texture);

        window.clear();
        window.draw(sprite);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}