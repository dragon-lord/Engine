#include "main.h"

void(*runningScreen)(float)=&test;
int quit;
struct Mesh* dets;
struct GuiElem *elements;
struct Mat4 perspMat;
struct Cam cam;
GLuint texture;
int model;

int main(int argc,char **argv){
  if(!Render_init("OpenGL",new(640,480)))
    return 1;

  perspMat=Mat4_projection(70,WINDOW.dim.x/WINDOW.dim.y,0.01,1000);
  cam=Cam_new(new(0,0,0),new(0,0,1),new(0,1,0));
  texture=Load_texture("res/bricks.jpg");
  model=glGetUniformLocation(mainShader,"model");

  // const GLfloat verts[]={
  //   -.5f,-.5f,3.f,
  //   -.5f,.5f,3.f,
  //   .5f,-.5f,3.f,
  //   .5f,.5f,3.f
  // };
  // const GLfloat colors[]={
  //   1.0f,0.0f,0.0f,
  //   0.0f,1.0f,0.0f,
  //   0.0f,0.0f,1.0f,
  //   1.0f,0.0f,1.0f
  // };
  // GLuint elems[6]={0,1,2,3,1,2};
  ALLOC_ARR(struct Mesh*,dets,1)
  dets[0]=LoadMesh("res/monkey.obj");//UploadMesh(verts,colors,elems,12,6);
  elements=Container_new(
    new(0,0),new(0,0),
    (struct GuiElem*[]){
      // Button_new(new(-100,-60),new(200,50),&button,NULL,0),
      // Button_new(new(-100,10),new(200,50),&button3,NULL,0)
      Button_new(new(-0.5,-0.45),new(1,0.25),new(0.5,0,0),NULL,&button,NULL,0),
      Button_new(new(-0.5,0.2),new(1,0.25),new(0,0.5,0),NULL,&button3,NULL,0)
    },
    2
  );

  while(!quit){
    Input_update();
    Render_update(runningScreen);
  }
  Elem_destroy(elements);
  elements=NULL;
  UnloadMesh(dets);
  glDeleteTextures(1,&texture);

  Render_destroy();
  return 0;
}

int esc=0;

void game(float fps){
  if(keys[6]&&!esc){
    button2();
    esc=1;
  }else if(!keys[6]&&esc)esc=0;
  cam=Cam_update(cam,fps);
  glUseProgram(mainShader);
  struct Mat4 look=Mat4_lookat(cam.pos,cam.fwd,cam.up);
  struct Mat4 mat=Mat4_mul(perspMat,look);
  // printf("[%f,%f,%f,%f/%f,%f,%f,%f/%f,%f,%f,%f/%f,%f,%f,%f]\n",
  // mat.f[0],mat.f[1],mat.f[2],mat.f[3],mat.f[4],mat.f[5],mat.f[6],mat.f[7],mat.f[8],mat.f[9],mat.f[10],mat.f[11],mat.f[12],mat.f[13],mat.f[14],mat.f[15]);
  glUniformMatrix4fv(model,1,GL_FALSE,&mat.f[0]);
  Bind_texture(0,texture);
  // glEnable(GL_DEPTH_TEST);
  DrawFull(dets);
  // elements->draw(elements);
  // elements->update(*elements);
}

void test(float fps){
  if(keys[6]&&!esc){
    button3();
    esc=1;
  }else if(!keys[6]&&esc)esc=0;
  glUseProgram(mainShader);
  struct Mat4 look=Mat4_lookat(cam.pos,cam.fwd,cam.up);
  struct Mat4 mat=Mat4_mul(perspMat,look);
  glUniformMatrix4fv(model,1,GL_FALSE,&mat.f[0]);
  Bind_texture(0,texture);
  // glEnable(GL_DEPTH_TEST);
  DrawFull(dets);
  // glDisable(GL_DEPTH_TEST);
  glClear(GL_DEPTH_BUFFER_BIT);
  elements->draw(*elements);
  elements->update(*elements);
}

void button(){
  quit=1;
}

void button2(){
  SDL_SetRelativeMouseMode(SDL_FALSE);
  SDL_WarpMouseInWindow(WINDOW.window,WINDOW.dim.x/2,WINDOW.dim.y/2);
  runningScreen=&test;
}

void button3(){
  SDL_SetRelativeMouseMode(SDL_TRUE);
  // SDL_ShowCursor(SDL_DISABLE);
  // SDL_ShowCursor(SDL_ENABLE);
  runningScreen=&game;
}
