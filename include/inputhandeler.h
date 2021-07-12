#ifndef INPUTHANDELER_H
#define INPUTHANDELER_H

#include "render.h"

extern SDL_Event e;

extern struct Vec2 mPos;
extern struct Vec2 mRel;
extern uint8_t mButton[5];
extern uint8_t keymap[7];
extern uint8_t keys[7];

void Input_update();

#endif //INPUTHANDELER_H
