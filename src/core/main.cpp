#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include "GLFW/glfw3.h"

#include "managers.hpp"

#include <iostream>

EventBus eventBus;



void error_callback(int error, const char* description);
int main();


int main()
{
    ShaderManager::Instance();
    Coordinator::Instance();   

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
    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (!window)
    {
        error_callback(-1, "Window was not created.");
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);

    float points[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    GLuint vbo = 0;
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, 9*sizeof(float), points, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 0, NULL);

    GLuint fs = ShaderManager::Instance().SetupShader("testf.frag");
    GLuint vs = ShaderManager::Instance().SetupShader("testv.vert");
    

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, fs);
    glAttachShader(shader_program, vs);
    glLinkProgram(shader_program);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram( shader_program );
        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLES, 0, 3);
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