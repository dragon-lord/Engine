#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#define CL_TARGET_OPENCL_VERSION 120
#include <CL/cl_gl.h>

int quit;

#include "maths.h"
#include "render.h"

int windowinit();
void game(float fps);

#endif //MAIN_H
