#include "inputhandeler.h"

struct Vec2 mPos;
struct Vec2 mRel;
uint8_t mButton[5];
uint8_t keymap[7]={26,22,7,4,44,225,41};
uint8_t keys[7];

void Input_update(){
  mRel=VEC2_ZERO;
  while(SDL_PollEvent(&e)){
    switch(e.type){
      case SDL_QUIT:
        quit=1;
      break;
      case SDL_MOUSEMOTION:
        mPos.x=e.motion.x;
        mPos.y=e.motion.y;
        mRel.x+=e.motion.xrel;
        mRel.y+=e.motion.yrel;
        mRel=Vec2_divn(mRel,2);
      break;
      case SDL_MOUSEBUTTONDOWN:
        mButton[e.button.button]=1;
      break;
      case SDL_MOUSEBUTTONUP:
        mButton[e.button.button]=0;
      break;
      case SDL_KEYDOWN:
      // printf("%i\n",e.button.button);
        for(int i=0;i<7;i++){
          // printf("%i\n",keymap[i]);
          if(e.button.button==keymap[i])
            keys[i]=1;
        }
      break;
      case SDL_KEYUP:
        for(int i=0;i<7;i++){
          if(e.button.button==keymap[i])
            keys[i]=0;
        }
      break;
    }
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
        int resolutionLocation=glGetUniformLocation(mainShader,"resolution");
        glUniform2f(resolutionLocation,WINDOW.dim.x,WINDOW.dim.y);
        perspMat=Mat4_projection(70,WINDOW.dim.x/WINDOW.dim.y,0.01,1000);
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
}//*/
