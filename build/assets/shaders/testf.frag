#version 430 core
out vec4 frag_color;
void main(){
   frag_color = vec4(gl_FragCoord.z, 0.5, 0, 1.0);
}