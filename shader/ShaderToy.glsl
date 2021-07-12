#version 330 core
out vec4 fragColor;
uniform vec2 resolution=vec2(640,480);

void main(){
  vec3 color=vec3(0);
  if(gl_FragCoord.x>resolution.x/2){
    color.x=.7;
  }if(gl_FragCoord.y>resolution.y/2){
    color.y=.7;
  }
  fragColor=vec4(color,1.0);
}
