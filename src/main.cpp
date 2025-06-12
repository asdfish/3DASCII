#include <imgui.h>
#include <imgui_impl_SDL3.h>
#include <imgui_impl_sdlrenderer3.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

#include "modelHandling.hpp"
#include "renderHandling.hpp"
#include "camera.hpp"

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

constexpr int wWidth = 125;
constexpr int wHeight = 250;
constexpr int scaleFactor = 4;

SDL_FRect dstRect = {
    0.0f, 0.0f,
    static_cast<float>(wWidth * scaleFactor),
    static_cast<float>(wHeight * scaleFactor)
};

float FOV = 30.f;
float focalLength = 1.f;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* texture = nullptr;

float zbuffer[wHeight*wWidth];
std::uint8_t pixelBuffer[wHeight*wWidth*4];

Camera camera(float3(), float3(), 30.f, 1.f, wWidth, wHeight);
Scene scene;


/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{

    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("3DASCII", wWidth*scaleFactor, wHeight*scaleFactor, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    camera = Camera(float3(), float3(), FOV, focalLength, wWidth, wHeight);
    objImporter("test.obj", scene);
    scene.GetObjectList();
    scene.GetObjectData();

    texture = SDL_CreateTexture(renderer,
    SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, wWidth, wHeight);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST); 

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io; // Avoid unused variable warning
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    ImGui_ImplSDL3_ProcessEvent(event);
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    const double now = ((double)SDL_GetTicks()) / 1000.0;  /* convert from milliseconds to seconds. */

    SDL_SetRenderDrawColorFloat(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(renderer);

    std::fill(pixelBuffer, pixelBuffer + (wWidth * wHeight * 4), 0);
    std::fill(zbuffer, zbuffer + (wWidth * wHeight), FLT_MAX);

    RenderScene(scene, camera, pixelBuffer, zbuffer);

    SDL_UpdateTexture(texture, nullptr, pixelBuffer, wWidth * 4);

    float3 cameraRot = camera.GetTransform().GetRot();
    float3 cameraPos = camera.GetTransform().GetPos();

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
    ImGui::ShowDemoWindow();

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
    ImGui::Begin("Camera Controls");
    ImGui::DragFloat3("Camera Rotation", &cameraRot.x, 0.1f, 0.f, 2.f*M_PI, "%.2f rad", ImGuiSliderFlags_WrapAround);
    ImGui::DragFloat3("Camera Position", &cameraPos.x);
    ImGui::End();

    camera.GetModifiableTransform().SetRot(cameraRot);
    camera.GetModifiableTransform().SetPos(cameraPos);

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
    ImGui::Begin("Models");
    for (auto& obj : scene.GetObjects())
    {
        ImGui::Text("%s", obj.GetName().c_str());
        float3 pos = obj.GetTransform().GetPos();
        float3 rot = obj.GetTransform().GetRot();
        ImGui::DragFloat3(("Position##" + obj.GetName()).c_str(), &pos.x, 0.05f);
        ImGui::DragFloat3(("Rotation##" + obj.GetName()).c_str(), &rot.x, 0.1f, 0.0f, 2.f*M_PI, "%.2f rad", ImGuiSliderFlags_WrapAround);
        ImGui::Separator();
        obj.GetTransform().SetPos(pos);
        obj.GetTransform().SetRot(rot);
    }
    ImGui::End();
    


    SDL_RenderTexture(renderer, texture, nullptr, &dstRect);

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

