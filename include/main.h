#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_opengl.h>
#include <GL/glew.h>
// #include <OpenGL-01Config.h>

extern int quit;
extern void(*runningScreen)(float);
extern struct Mat4 perspMat;
extern struct Cam cam;

#include "maths.h"
#include "render.h"
#include "cam.h"

void game(float fps);
void test(float fps);

void button();
void button2();
void button3();

#endif //MAIN_H
