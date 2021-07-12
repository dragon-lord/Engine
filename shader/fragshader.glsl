#version 330 core
out vec4 color;
in vec2 fragTex;
in vec3 fragNorm;
uniform sampler2D diffuse;
uniform vec3 light=vec3(1,-1,1);

void main(){
    color=texture2D(diffuse,fragTex)
      *clamp(dot(-light,fragNorm),0.25,1.0);
}
