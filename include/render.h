#ifndef RENDER_H
#define RENDER_H

#include "main.h"
#include "inputhandeler.h"
#include "gui.h"

struct Window{
  SDL_Window *window;
  SDL_GLContext *context;
  struct Vec2 dim;
  GLuint Tex;
  int state;
  int ID;
};

struct Mesh{
  uint32_t vao;
  uint32_t numElements;
};

struct Texture{
  uint32_t tex;
  float aspect;
};

extern struct Window WINDOW;
extern uint mainShader;
extern uint guiShader;
extern TTF_Font *font;

int Render_init(char *title,struct Vec2 dim);
void Render_update(void(*update)(float));

struct Mesh ScreenMesh();
struct Mesh UploadMesh(const GLfloat *verts,const GLfloat *colors,const GLuint *elems,int vertSize,int elemSize);
struct Mesh UploadMeshFull(const GLfloat *verts,const GLfloat *tex,const GLfloat *norm,int vertSize);
struct Mesh LoadMesh(const char *file);
void Draw(struct Mesh *details);
void DrawFull(struct Mesh *details);
void UnloadMesh(struct Mesh *details);

GLuint LoadShader(const char* vertexshader, const char* fragmentshader);

GLuint Load_text(char *text,struct Vec3 c);
GLuint Load_texture(char *path);
void Bind_texture(uint unit,GLuint texture);

void printGLinfo();
void Render_destroy();

#endif //RENDER_H
