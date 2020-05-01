#include "render.h"

float frameTime;
int prevTime;
int currentTime;
float deltaTime;
GLuint FBO;

GLuint program;

int Render_init(int num,int(*winit)()){
  if(SDL_Init(SDL_INIT_VIDEO)<0){
    printf("Failed to init SDL\n");
    return 0;
  }
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);
  size_t *raw=malloc(sizeof(size_t)+num*sizeof(struct Window));
  raw[0]=num;
  WINDOWS=(struct Window*)&raw[1];
  if(!(*winit)())
    return 0;

  glewExperimental=GL_TRUE;
  if(glewInit()!=GLEW_OK){
    printf("ERR: glewInit\n");
    return 0;
  }/*

  FBO=0;
  glGenFramebuffers(1,&FBO);
  glBindFramebuffer(GL_FRAMEBUFFER,FBO);

  GLuint renderedTexture;
  glGenTextures(1,&renderedTexture);

  glBindTexture(GL_TEXTURE_2D,renderedTexture);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,1024,768,0,GL_RGB,GL_UNSIGNED_BYTE,0);

  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

  GLuint depthrenderbuffer;
  glGenRenderbuffers(1,&depthrenderbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER,depthrenderbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,1024,768);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthrenderbuffer);

  // Set "renderedTexture" as our colour attachement #0
  glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,renderedTexture,0);

  // Set the list of draw buffers.
  GLenum DrawBuffers[1]={GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1,DrawBuffers); // "1" is the size of DrawBuffers

  // Always check that our framebuffer is ok
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE){
    printf("GLFramebuffer could not be created!\n");
    Render_destroy();
    / *SDL_GL_DeleteContext(win.context);
    win.context=NULL;
    SDL_DestroyWindow(win.window);
    win.window=NULL;// * /
    return 0;
  }//*/

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_DEPTH_TEST);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  program=GLCreateShaderProgram(0);
  return 1;
}

struct Window Window_init(char *title,struct Vec2 dim){
  struct Window win;
  win.window=SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,dim.x,dim.y,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
  if(win.window==NULL){
    printf("Window could not be created! SDL Error: %s\n",SDL_GetError());
    return win;
  }
  win.context=SDL_GL_CreateContext(win.window);
  if(win.context==NULL){
    printf("GLContext could not be created! SDL Error: %s\n",SDL_GetError());
    SDL_DestroyWindow(win.window);
    win.window=NULL;
    return win;
  }
  win.dim=dim;
  win.ID=SDL_GetWindowID(win.window);
  win.state=0b110011;

  return win;
}

void Window_update(struct Window *win,void(*update)(float)){
  SDL_GL_MakeCurrent(win->window,win->context);
  glClearColor(0.25,0.05,0.15,1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  prevTime=currentTime;
  currentTime=SDL_GetTicks();
  deltaTime=(currentTime-prevTime)/1000.0f;

  (*update)(1/deltaTime);

  glViewport(0,0,win->dim.x,win->dim.y);
  SDL_GL_SwapWindow(win->window);
  SDL_Delay(10);
}

void Fill_triangle(struct Vec2 pnts[3],Uint32 color){
	float r=((float)(Uint8)(color >> 16))/255;
	float g=((float)(Uint8)(color >> 8))/255;
	float b=((float)(Uint8)(color))/255;
	GLfloat vertices[]={
		pnts[0].x,pnts[0].y,0,	r,g,0.25,
		pnts[1].x,pnts[1].y,0,	0.25,g,b,
		pnts[2].x,pnts[2].y,0,	r,0.25,b
	};

  GLuint VBO,VAO;
  glGenVertexArrays(1,&VAO);
  glGenBuffers(1,&VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER,VBO);
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),(GLvoid*)0);
  glEnableVertexAttribArray(0);

	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER,0);
  //glBindVertexArray(0);

	glUseProgram(program);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES,0,3);
	glBindVertexArray(0);

  glDeleteVertexArrays(1,&VAO);
  glDeleteBuffers(1,&VBO);
}

void Window_destroy(struct Window *win){
  if(win->context!=NULL){
    SDL_GL_DeleteContext(win->context);
    win->context=NULL;
  }if(win->window!=NULL){
    SDL_DestroyWindow(win->window);
    win->window=NULL;
  }if(FBO!=0){
    glDeleteFramebuffers(1,&FBO);
    FBO=0;
  }
}

void Render_destroy(){
  for(int i=0;i<GET_DIM(WINDOWS);i++){
    Window_destroy(&WINDOWS[i]);
  }
  glDeleteProgram(program);
  free((size_t*)(WINDOWS)-1);
  SDL_Quit();
}
