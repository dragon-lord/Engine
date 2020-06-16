#include "render.h"

void Input_update(){
  while(SDL_PollEvent(&e)){
    if(e.type==SDL_QUIT)
      quit=1;
    if(!(e.type==SDL_WINDOWEVENT && e.window.windowID==WINDOW.ID))
      continue;
    switch(e.window.event){
      //Window appeared
      case SDL_WINDOWEVENT_SHOWN:
        WINDOW.state|=0b000010;
        //mShown=true;
      break;

      //Window disappeared
      case SDL_WINDOWEVENT_HIDDEN:
        WINDOW.state&=0b111101;
        //mShown=false;
      break;

      //Get new dimensions and repaint
      case SDL_WINDOWEVENT_SIZE_CHANGED:
        WINDOW.dim=VEC2_NEW(e.window.data1,e.window.data2);
        //mWidth=e.window.data1;
        //mHeight=e.window.data2;
        //SDL_RenderPresent(WINDOW.renderer);
      break;

      //Repaint on expose
      case SDL_WINDOWEVENT_EXPOSED:
        //SDL_RenderPresent(WINDOW.renderer);
      break;

      //Mouse enter
      case SDL_WINDOWEVENT_ENTER:
        WINDOW.state|=0b100000;
        //mMouseFocus=true;
        //updateCaption=true;
      break;

      //Mouse exit
      case SDL_WINDOWEVENT_LEAVE:
        WINDOW.state&=0b011111;
        //mMouseFocus=false;
        //updateCaption=true;
      break;

      //Keyboard focus gained
      case SDL_WINDOWEVENT_FOCUS_GAINED:
        WINDOW.state|=0b010000;
        //mKeyboardFocus=true;
        //updateCaption=true;
      break;

      //Keyboard focus lost
      case SDL_WINDOWEVENT_FOCUS_LOST:
        WINDOW.state&=0b101111;
        //mKeyboardFocus=false;
        //updateCaption=true;
      break;

      //Window minimized
      case SDL_WINDOWEVENT_MINIMIZED:
        WINDOW.state|=0b000100;
        //mMinimized=true;
      break;

      //Window maxized
      case SDL_WINDOWEVENT_MAXIMIZED:
        WINDOW.state&=0b111011;
        //mMinimized=false;
      break;

      //Window restored
      case SDL_WINDOWEVENT_RESTORED:
        WINDOW.state&=0b111011;
        //mMinimized=false;
      break;

      //Hide on close
      case SDL_WINDOWEVENT_CLOSE:
        quit=1;
      break;
    }
  }
}
