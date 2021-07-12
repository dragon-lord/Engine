#version 330 core
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 vertTex;
layout (location = 2) in vec3 vertNorm;

out vec2 fragTex;
out vec3 fragNorm;

uniform mat4 model;

void main(){
    gl_Position=model*vec4(vertPos,1.0f);
    fragTex=vertTex;
    fragNorm=vertNorm;
}
