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

EventBus eventBus;



void error_callback(int error, const char* description);
int main();


int main()
{

    /* Setup */
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
    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (!window)
    {
        error_callback(-1, "Window was not created.");
        glfwTerminate();
        return 0;
    }
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    std::cout<<"Window size: "<<width<<"x"<<height<<"\n";
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_CULL_FACE));
    GLCall(glCullFace(GL_BACK));
    GLCall(glFrontFace(GL_CCW));
    GLCall(glDepthFunc(GL_LESS));
    GLCall(glViewport(0, 0, width, height)); 


    RenderContext::Instance().FOV = 50.f;
    RenderContext::Instance().nearPlane = 0.01f;
    RenderContext::Instance().farPlane = 100.f;
    RenderContext::Instance().aspect = 640.f/480.f;
    RenderContext::Instance().camTransform.position = glm::vec3(1.f, 1.f, 10.f);
    RenderContext::Instance().camTransform.rotation = glm::quat(1.f, 0.f, 0.f, 0.f);
    RenderContext::Instance().ResetProjMatrix();
    RenderContext::Instance().ResetViewMatrix();

    AssetManager::Instance();
    Coordinator::Instance();  

    //Coordinator Initialize: Components
    Coordinator::Instance().RegisterComponent<MeshData>();
    Coordinator::Instance().RegisterComponent<MeshGPU>();
    Coordinator::Instance().RegisterComponent<Transform>();

    //Coordinator Initialize: Systems
    auto moveSys = Coordinator::Instance().RegisterSystem<RenderSystem>();
    Signature sig;
    sig.set(Coordinator::Instance().GetComponentType<MeshGPU>());
    sig.set(Coordinator::Instance().GetComponentType<Transform>());
    Coordinator::Instance().SetSignature<RenderSystem>(sig);

    AssetManager::Instance().MeshImport("DirectionTest.obj");
    AssetManager::Instance().MeshLoad("-X");
    AssetManager::Instance().MeshLoad("+X");
    AssetManager::Instance().MeshLoad("-Y");
    AssetManager::Instance().MeshLoad("+Y");
    AssetManager::Instance().MeshLoad("-Z");
    AssetManager::Instance().MeshLoad("+Z");


    AssetManager::Instance().CreateShaderProgram("test", {"testf.frag", "testv.vert"});

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        
        moveSys -> Update();

        glfwSwapBuffers(window);
    }

    /*End*/
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