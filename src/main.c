#include "main.h"
/*
#if defined(SDL_VIDEO_DRIVER_WINDOWS)
#define TEST "WINDOWS"
#elif defined(SDL_VIDEO_DRIVER_WINRT)
#define TEST "WINRT"
#elif defined(SDL_VIDEO_DRIVER_X11)
#define TEST "X11"
#elif defined(SDL_VIDEO_DRIVER_DIRECTFB)
#define TEST "DIRECTFB"
#elif defined(SDL_VIDEO_DRIVER_COCOA)
#define TEST "COCOA"
#elif defined(SDL_VIDEO_DRIVER_UIKIT)
#define TEST "UIKIT"
#elif defined(SDL_VIDEO_DRIVER_WAYLAND)
#define TEST "WAYLAND"
#elif defined(SDL_VIDEO_DRIVER_MIR)
#define TEST "MIR"
#elif defined(SDL_VIDEO_DRIVER_ANDROID)
#define TEST "ANDROID"
#elif defined(SDL_VIDEO_DRIVER_VIVANTE)
#define TEST "VIVANTE"
#endif//*/
/*#if defined(SDL_VIDEO_DRIVER_WINDOWS)
#define DIPLAY_TYPE CL_WGL_HDC_KHR
#elif defined(SDL_VIDEO_DRIVER_WINRT)
#define DIPLAY_TYPE "WINRT"
#elif defined(SDL_VIDEO_DRIVER_X11)
#define DIPLAY_TYPE CL_GLX_DISPLAY_KHR
#elif defined(SDL_VIDEO_DRIVER_DIRECTFB)
#define DIPLAY_TYPE "DIRECTFB"
#elif defined(SDL_VIDEO_DRIVER_COCOA)
#define DIPLAY_TYPE "COCOA"
#elif defined(SDL_VIDEO_DRIVER_UIKIT)
#define DIPLAY_TYPE "UIKIT"
#elif defined(SDL_VIDEO_DRIVER_WAYLAND)
#define DIPLAY_TYPE "WAYLAND"
#elif defined(SDL_VIDEO_DRIVER_MIR)
#define DIPLAY_TYPE "MIR"
#elif defined(SDL_VIDEO_DRIVER_ANDROID)
#define DIPLAY_TYPE "ANDROID"
#elif defined(SDL_VIDEO_DRIVER_VIVANTE)
#define DIPLAY_TYPE "VIVANTE"
#endif//*/

void temp(){
  //printf("%s\n",TEST);
  SDL_SysWMinfo info;
  SDL_VERSION(&info.version);
  if(!SDL_GetWindowWMInfo(WINDOWS[0].window,&info)){
    printf("%s\n",SDL_GetError());
    return;
  }
  cl_platform_id platform_id = NULL;
  cl_device_id device_id = NULL;
  cl_uint ret_num_devices;
  cl_uint ret_num_platforms;
  cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
  ret=clGetDeviceIDs(platform_id,CL_DEVICE_TYPE_DEFAULT,1,&device_id, &ret_num_devices);

  cl_context_properties properties[]={
    CL_CONTEXT_PLATFORM,(cl_context_properties)platform_id,
  	CL_GL_CONTEXT_KHR,(cl_context_properties)WINDOWS[0].context,
  	CL_GLX_DISPLAY_KHR,(cl_context_properties)WINDOWS[0].window,
  	0
	};
  cl_context ctx=clCreateContext(properties,1,&device_id,NULL,NULL,&ret);
  if(ret!=0){
	 printf("Context creation => OpenCL error: %d\n",ret);
   ret=clReleaseContext(ctx);//*/
   return;
  }

  // Create a command queue
  cl_command_queue queue=clCreateCommandQueue(ctx,device_id,0,&ret);

  //Create Image
	cl_mem image=clCreateFromGLTexture(ctx,CL_MEM_READ_WRITE,GL_TEXTURE_2D,0,WINDOWS[0].Tex,&ret);
	if(ret!=0){
		printf("Texture creation =>  OpenCL error: %d\n",ret);
    ret=clReleaseMemObject(image);
    ret=clFlush(queue);
    ret=clFinish(queue);
    ret=clReleaseContext(ctx);
    return;
  }
  ret=clReleaseMemObject(image);
  ret=clFlush(queue);
  ret=clFinish(queue);
  ret=clReleaseContext(ctx);
}

int main(int argc,char **argv){
  if(!Render_init(1,&windowinit))
    return 1;

  temp();

  while(!quit){
    Input_update();
    Window_update(&WINDOWS[0],&game);

    int closed=1;
    for(int i=0;i<GET_DIM(WINDOWS);i++)
      if(WINDOWS[i].state&0b000001)
        closed=0;
    if(closed)
      quit=1;
  }

  Render_destroy();
  return 0;
}

int windowinit(){
  WINDOWS[0]=Window_init("test",new(640,480));
  if(WINDOWS[0].window==NULL){
    Render_destroy();
    return 0;
  }
  return 1;
}

void game(float fps){
  struct Vec2 pnts[3]={
    new(-1,-1),
    new(1,-1),
    new(0,1)
  };
  Fill_triangle(pnts,0xFF888888);
}
