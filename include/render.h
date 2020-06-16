#ifndef RENDER_H
#define RENDER_H

#include "main.h"

struct Window{
  SDL_Window *window;
  SDL_GLContext *context;
  struct Vec2 dim;
  //Uint32 *pixels;
  GLuint FBO;
  GLuint Tex;
  int state;
  int ID;
};

struct Window WINDOW;
SDL_Event e;

int Render_init(char *title,struct Vec2 dim);
void Render_update(void(*update)(float));

GLuint Load_texture(char *path);
GLuint Load_texture3(char *path);
GLuint Load_text(char *text,Uint32 c);

void Fill_triangle(struct Vec2 pnts[3],Uint32 color);
void Render_texture(struct Vec2 pnts[2],GLuint texture);

void Render_destroy();


void Input_update();


GLuint GLCreateShaderProgram(int s);

#endif //RENDER_H
