#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include "GLFW/glfw3.h"

#include "managers.hpp"
#include "rendercomponents.hpp"
#include "RenderContext.hpp"

#include "components.hpp"
#include "systems.hpp"

#include "errorHandle.hpp"

#include <iostream>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuiFileDialog.h>

EventBus eventBus;



void error_callback(int error, const char* description);
int main();


int main()
{
    /* GLFW Setup */

    if (!glfwInit())
    {
        error_callback(-1, "GLFW was not initialized.");
        return 0;
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    // Create window with graphics context
    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only
    GLFWwindow* window = glfwCreateWindow((int)(1280 * main_scale), (int)(800 * main_scale), "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_CULL_FACE));
    GLCall(glCullFace(GL_BACK));
    GLCall(glFrontFace(GL_CCW));
    GLCall(glDepthFunc(GL_LESS));
    GLCall(glViewport(0, 0, width, height)); 

    /* ImGUI Setup */

    const char* glsl_version = "#version 130";



    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    /*Code*/
    RenderContext::Instance().FOV = 50.f;
    RenderContext::Instance().nearPlane = 0.01f;
    RenderContext::Instance().farPlane = 100.f;
    RenderContext::Instance().aspect = 640.f/480.f;
    RenderContext::Instance().camTransform.position = glm::vec3(1.f, 1.f, 10.f);
    RenderContext::Instance().camTransform.rotation = glm::quat(1.f, 0.f, 0.f, 0.f);

    RenderContext::Instance().scrollVel = 2.f;
    RenderContext::Instance().panVel = 0.2f;
    RenderContext::Instance().rotVel = 0.001f;
    RenderContext::Instance().FOVChangeSpeed = 6.f;
    RenderContext::Instance().motionVel = 2.f;

    RenderContext::Instance().ResetProjMatrix();
    RenderContext::Instance().ResetViewMatrix();

    AssetManager::Instance();
    Coordinator::Instance();  

    //Coordinator Initialize: Components
    Coordinator::Instance().RegisterComponent<MeshData>();
    Coordinator::Instance().RegisterComponent<MeshGPU>();
    Coordinator::Instance().RegisterComponent<Transform>();
    Coordinator::Instance().RegisterComponent<ID>();

    //Coordinator Initialize: Systems
    auto renderSys = Coordinator::Instance().RegisterSystem<RenderSystem>();
    Signature renderSig;
    renderSig.set(Coordinator::Instance().GetComponentType<MeshGPU>());
    renderSig.set(Coordinator::Instance().GetComponentType<Transform>());
    Coordinator::Instance().SetSignature<RenderSystem>(renderSig);

    auto UISys = Coordinator::Instance().RegisterSystem<UISystem>();
    Signature UISig;
    UISig.set(Coordinator::Instance().GetComponentType<MeshGPU>());
    UISig.set(Coordinator::Instance().GetComponentType<Transform>());
    UISig.set(Coordinator::Instance().GetComponentType<ID>());
    Coordinator::Instance().SetSignature<UISystem>(UISig);


    AssetManager::Instance().CreateShaderProgram("test", {"testf.frag", "testv.vert"});

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();

                AssetManager::Instance().MeshImport(filePath.c_str()); 
            }

            ImGuiFileDialog::Instance()->Close();
        }

        ImGuiID id = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_NoDockingInCentralNode | ImGuiDockNodeFlags_PassthruCentralNode, nullptr);
        ImGuiDockNode* node = ImGui::DockBuilderGetCentralNode(id);

        //Clear the screen
        GLCall(glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        
        //Resize the window
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        GLCall(glViewport(0, 0, display_w, display_h));
        RenderContext::Instance().aspect = static_cast<float>(display_w)/display_h;

        //Systems
        renderSys -> Update();
        UISys -> Update();
        
        //Draw UI
        ImGui::ShowDemoWindow();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }




    /*End*/
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

/*
    float points[] = {
        0.5f, 0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 1.0f
    };

    GLuint indices[] = {
        0,2,1,
        0,3,2
    };

        VertexArray va;
    VertexBuffer vb(points, 4*4*sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(4);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);
    



    const char* vertex_shader = 
    "#version 430 core\n"
    "in vec3 vp;"
    "void main() {"
    "   gl_Position = vec4 (vp, 1.0);"
    "}";

    const char* fragment_shader = 
    "#version 430 core\n"
    "out vec4 frag_color;"
    "void main(){"
    "   frag_color = vec4(0.5, 0.0, 0.5, 1.0);"
    "}";

    GLuint vs = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);


*/