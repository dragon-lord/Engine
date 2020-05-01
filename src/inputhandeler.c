#include "render.h"

void Input_update(){
  while(SDL_PollEvent(&e)){
    if(e.type==SDL_QUIT)
      quit=1;
    for(int i=0;i<GET_DIM(WINDOWS);i++){
      WinInput_update(&WINDOWS[i],e);
    }
  }
}

void WinInput_update(struct Window *win,SDL_Event e){
  if(!(e.type==SDL_WINDOWEVENT && e.window.windowID==win->ID))
    return;
  switch(e.window.event){
    //Window appeared
    case SDL_WINDOWEVENT_SHOWN:
      win->state|=0b000010;
      //mShown=true;
    break;

    //Window disappeared
    case SDL_WINDOWEVENT_HIDDEN:
      win->state&=0b111101;
      //mShown=false;
    break;

    //Get new dimensions and repaint
    case SDL_WINDOWEVENT_SIZE_CHANGED:
      win->dim=VEC2_NEW(e.window.data1,e.window.data2);
      //mWidth=e.window.data1;
      //mHeight=e.window.data2;
      //SDL_RenderPresent(win->renderer);
    break;

    //Repaint on expose
    case SDL_WINDOWEVENT_EXPOSED:
      //SDL_RenderPresent(win->renderer);
    break;

    //Mouse enter
    case SDL_WINDOWEVENT_ENTER:
      win->state|=0b100000;
      //mMouseFocus=true;
      //updateCaption=true;
    break;

    //Mouse exit
    case SDL_WINDOWEVENT_LEAVE:
      win->state&=0b011111;
      //mMouseFocus=false;
      //updateCaption=true;
    break;

    //Keyboard focus gained
    case SDL_WINDOWEVENT_FOCUS_GAINED:
      win->state|=0b010000;
      //mKeyboardFocus=true;
      //updateCaption=true;
    break;

    //Keyboard focus lost
    case SDL_WINDOWEVENT_FOCUS_LOST:
      win->state&=0b101111;
      //mKeyboardFocus=false;
      //updateCaption=true;
    break;

    //Window minimized
    case SDL_WINDOWEVENT_MINIMIZED:
      win->state|=0b000100;
      //mMinimized=true;
    break;

    //Window maxized
    case SDL_WINDOWEVENT_MAXIMIZED:
      win->state&=0b111011;
      //mMinimized=false;
    break;

    //Window restored
    case SDL_WINDOWEVENT_RESTORED:
      win->state&=0b111011;
      //mMinimized=false;
    break;

    //Hide on close
    case SDL_WINDOWEVENT_CLOSE:
      win->state&=0b111110;
      SDL_HideWindow(win->window);
    break;
  }
}
