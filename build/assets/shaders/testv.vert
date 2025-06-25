#version 430 core

layout(location = 0) in vec4 vp;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main() {
   gl_Position = projMatrix * viewMatrix * worldMatrix * vp;
}