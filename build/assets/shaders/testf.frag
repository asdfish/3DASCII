#version 430 core

in vec3 normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 lightCol;
uniform vec3 objectCol;
uniform float ambient;

out vec4 frag_color;
void main(){
   vec3 norm = normalize(normal);
   vec3 lightDir = normalize(lightPos-FragPos);

   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightCol;
   vec3 result = (ambient+diffuse)*objectCol;
   frag_color = vec4(result, 1.0);
}