#pragma once
#include <glad/gl.h>
#include <iostream>
#include <csignal>

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout<<"[OpenGL Error] (" << error << "): " << function << " "<< file<< ":" << line<<"\n";
        return false;
    }
    return true;
}


#ifndef NDEBUG
    #define ASSERT(x) if (!(x)) __builtin_trap();
    #define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall((#x), __FILE__, __LINE__));
#else
    #define ASSERT(x) x;
    #define GLCall(x) x;
#endif

