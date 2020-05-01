#include "render.h"

float frameTime;
int prevTime;
int currentTime;
float deltaTime;

GLuint triprogram;
GLuint texprogram;

int Render_init(int num,int(*winit)()){
  if(SDL_Init(SDL_INIT_VIDEO)<0){
    printf("Failed to init SDL\n");
    return 0;
  }if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)<0){
    printf("Failed to init IMG\n");
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
  }
  for(int i=0;i<GET_DIM(WINDOWS);i++){
    Window_fbo(&WINDOWS[i]);
  }
  //Window_fbo(&WINDOWS[0]);
  //Window_fbo(&WINDOWS[1]);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_DEPTH_TEST);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  triprogram=GLCreateShaderProgram(0);
  texprogram=GLCreateShaderProgram(1);
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

int Window_fbo(struct Window *win){
  win->FBO=0;
  glGenFramebuffers(1,&win->FBO);
  glBindFramebuffer(GL_FRAMEBUFFER,win->FBO);

  //renderedTexture=Load_texture("resources/tex.png");
  //GLuint texColorBuffer;
  glGenTextures(1,&win->Tex);
  glBindTexture(GL_TEXTURE_2D,win->Tex);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,1024,768,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D,0);

  glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,win->FBO,0);

  GLenum DrawBuffers[1]={GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1,DrawBuffers);

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE){
    printf("GLFramebuffer could not be created!\n");
    Render_destroy();
    return 0;
  }
  return 1;
}

void Window_update(struct Window *win,void(*update)(float)){
  SDL_GL_MakeCurrent(win->window,win->context);
  glBindFramebuffer(GL_FRAMEBUFFER,win->FBO);
  //glBindFramebuffer(GL_FRAMEBUFFER,win->FBO);
  //glViewport(0,0,1024,768);
  glClearColor(0.25,0.05,0.15,1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  prevTime=currentTime;
  currentTime=SDL_GetTicks();
  deltaTime=(currentTime-prevTime)/1000.0f;
  glViewport(0,0,1024,768);

  (*update)(1/deltaTime);

  struct Vec2 pnts[2]={
    new(-1,-1),
    new(1,1)
  };
  glBindFramebuffer(GL_FRAMEBUFFER,0);
  glViewport(0,0,win->dim.x,win->dim.y);
  Render_texture(pnts,win->Tex);

  SDL_GL_SwapWindow(win->window);
  SDL_Delay(10);
}

GLuint Load_texture(char *path){
	GLuint texture;
  glGenTextures(1,&texture);
  glBindTexture(GL_TEXTURE_2D,texture);

  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

  SDL_Surface *surface=IMG_Load(path);
  if(surface==NULL){
    printf("Failed to load img\n");
    return texture;
  }
  surface=SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_ABGR8888,0);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,surface->w,surface->h,0,GL_RGBA,GL_UNSIGNED_BYTE,surface->pixels);
  glGenerateMipmap(GL_TEXTURE_2D);
  SDL_FreeSurface(surface);
  glBindTexture(GL_TEXTURE_2D,0);
	return texture;
}

GLuint Load_texture3(char *path){
  GLuint texture=0;
  return texture;
}

GLuint Load_text(char *text,Uint32 c){
  GLuint texture=0;
  return texture;
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

	glUseProgram(triprogram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES,0,3);
	glBindVertexArray(0);

  glDeleteVertexArrays(1,&VAO);
  glDeleteBuffers(1,&VBO);
}

void Render_texture(struct Vec2 pnts[2],GLuint texture){
  GLfloat vertices[]={
		pnts[0].x,pnts[0].y,0,	0,0,
		pnts[0].x,pnts[1].y,0,	0,1,
		pnts[1].x,pnts[1].y,0,	1,1,
		pnts[1].x,pnts[0].y,0,	1,0
	};
  GLuint indeces[]={
    0,2,1,
    0,3,2
  };

  GLuint VBO,VAO,EBO;
  glGenVertexArrays(1,&VAO);
  glGenBuffers(1,&VBO);
  glGenBuffers(1,&EBO);


  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER,VBO);
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indeces),indeces,GL_STATIC_DRAW);


  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

	glUseProgram(texprogram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,texture);
	glUniform1i(glGetUniformLocation(texprogram,"ourTexture"),0);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
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
  }if(win->FBO!=0){
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glDeleteFramebuffers(1,&win->FBO);
    win->FBO=0;
  }if(win->Tex!=0){
    glDeleteTextures(1,&win->Tex);
    win->Tex=0;
  }
}

void Render_destroy(){
  for(int i=0;i<GET_DIM(WINDOWS);i++){
    Window_destroy(&WINDOWS[i]);
  }
  glDeleteProgram(texprogram);
  glDeleteProgram(triprogram);
  free((size_t*)(WINDOWS)-1);
  IMG_Quit();
  SDL_Quit();
}