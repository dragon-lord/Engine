#version 330 core
out vec4 color;

uniform vec3 c=vec3(0.5,0,0);

void main(){
    color=vec4(c,1.0);
}
