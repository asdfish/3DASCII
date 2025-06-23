#version 430 core
layout(location = 0) in vec4 vp;

void main() {
   gl_Position = vp;
}