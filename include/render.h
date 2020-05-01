#ifndef RENDER_H
#define RENDER_H

#include "main.h"

struct Window *WINDOWS;
SDL_Event e;

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

int Render_init(int num,int(*winit)());
struct Window Window_init(char *title,struct Vec2 dim);
int Window_fbo(struct Window *win);
void Window_update(struct Window *win,void(*update)(float));

GLuint Load_texture(char *path);
GLuint Load_texture3(char *path);
GLuint Load_text(char *text,Uint32 c);

void Fill_triangle(struct Vec2 pnts[3],Uint32 color);
void Render_texture(struct Vec2 pnts[2],GLuint texture);

void Window_destroy(struct Window *win);
void Render_destroy();


void Input_update();
void WinInput_update(struct Window *win,SDL_Event e);


GLuint GLCreateShaderProgram(int s);

#endif //RENDER_H
