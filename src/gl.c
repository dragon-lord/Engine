#include "render.h"

const char *vertexCode=
"#version 330 core\n"
"layout (location=0) in vec3 position;\n"
"layout (location=1) in vec3 color;\n"
"out vec3 ourColor;\n"
"void main(){\n"
"  gl_Position=vec4(position,1.0);\n"
"  ourColor=color;\n"
"}\n";

char fragmentCode[]=
"#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 color;\n"
"void main(){\n"
"  color=vec4(ourColor,1.0);\n"
"}\n";

char *vtexCode=
"#version 330 core\n"
"layout (location=0) in vec3 position;\n"
"layout (location=1) in vec2 texCoord;\n"
"out vec2 TexCoord;\n"
"void main(){\n"
"  gl_Position=vec4(position,1.0);\n"
"  TexCoord=texCoord;\n"
"}\n";

char *ftexCode=
"#version 330 core\n"
"in vec2 TexCoord;\n"
"out vec4 color;\n"
"uniform sampler2D ourTexture1;\n"
"void main(){\n"
"  color=texture(ourTexture1,TexCoord);\n"
"}\n";

GLuint GLCreateShaderProgram(int s){
  GLuint Program;
  const GLchar *vShaderCode;
  const GLchar *fShaderCode;
  if(s==0){
    vShaderCode=vertexCode;
    fShaderCode=fragmentCode;
  }else if(s==1){
    vShaderCode=vtexCode;
    fShaderCode=ftexCode;
  }
  GLuint vertex, fragment;
  GLint success;
  GLchar infoLog[512];

  vertex=glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex,1,&vShaderCode,NULL);
  glCompileShader(vertex);
  glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
  if(!success){
    glGetShaderInfoLog(vertex,512,NULL,infoLog);
    printf("ERR: glCompileShader - vertexShader\n  %s\n",infoLog);
  }

  fragment=glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment,1,&fShaderCode,NULL);
  glCompileShader(fragment);
  glGetShaderiv(fragment,GL_COMPILE_STATUS,&success);
  if(!success){
    glGetShaderInfoLog(fragment,512,NULL,infoLog);
    printf("ERR: glCompileShader - fragmentShader\n  %s\n",infoLog);
  }

  Program=glCreateProgram();
  glAttachShader(Program,vertex);
  glAttachShader(Program,fragment);
  glLinkProgram(Program);
  glGetProgramiv(Program,GL_LINK_STATUS,&success);
  if(!success){
    glGetProgramInfoLog(Program,512,NULL,infoLog);
    printf("ERR: glLinkProgram\n  %s\n",infoLog);
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
  return Program;
}
