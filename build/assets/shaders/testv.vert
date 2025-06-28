#version 430 core

layout(location = 0) in vec4 vp;
layout(location = 1) in vec3 vn;

out vec3 normal;
out vec3 FragPos;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main() {
   
   gl_Position = projMatrix * viewMatrix * worldMatrix * vp;
   FragPos = vec3(worldMatrix * vp);
   normal = vn;
}