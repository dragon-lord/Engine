#include "render.h"

float frameTime;
int prevTime;
int currentTime;
float deltaTime;

GLuint triprogram;
GLuint texprogram;

int Render_init(char *title,struct Vec2 dim){
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

  WINDOW.window=SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,dim.x,dim.y,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
  if(WINDOW.window==NULL){
    printf("Window could not be created! SDL Error: %s\n",SDL_GetError());
    return 0;
  }
  WINDOW.context=SDL_GL_CreateContext(WINDOW.window);
  if(WINDOW.context==NULL){
    printf("GLContext could not be created! SDL Error: %s\n",SDL_GetError());
    SDL_DestroyWindow(WINDOW.window);
    WINDOW.window=NULL;
    return 0;
  }
  WINDOW.dim=dim;
  WINDOW.ID=SDL_GetWindowID(WINDOW.window);
  WINDOW.state=0b110011;

  glewExperimental=GL_TRUE;
  if(glewInit()!=GLEW_OK){
    printf("ERR: glewInit\n");
    return 0;
  }

  WINDOW.FBO=0;
  glGenFramebuffers(1,&WINDOW.FBO);
  glBindFramebuffer(GL_FRAMEBUFFER,WINDOW.FBO);

  //renderedTexture=Load_texture("resources/tex.png");
  //GLuint texColorBuffer;
  glGenTextures(1,&WINDOW.Tex);
  glBindTexture(GL_TEXTURE_2D,WINDOW.Tex);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,1024,768,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D,0);

  glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,WINDOW.FBO,0);

  GLenum DrawBuffers[1]={GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1,DrawBuffers);

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE){
    printf("GLFramebuffer could not be created!\n");
    Render_destroy();
    return 0;
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

void Render_update(void(*update)(float)){
  SDL_GL_MakeCurrent(WINDOW.window,WINDOW.context);
  glBindFramebuffer(GL_FRAMEBUFFER,WINDOW.FBO);
  //glBindFramebuffer(GL_FRAMEBUFFER,WINDOW.FBO);
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
  glViewport(0,0,WINDOW.dim.x,WINDOW.dim.y);
  Render_texture(pnts,WINDOW.Tex);

  SDL_GL_SwapWindow(WINDOW.window);
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

void Render_destroy(){
  if(WINDOW.context!=NULL){
    SDL_GL_DeleteContext(WINDOW.context);
    WINDOW.context=NULL;
  }if(WINDOW.window!=NULL){
    SDL_DestroyWindow(WINDOW.window);
    WINDOW.window=NULL;
  }if(WINDOW.FBO!=0){
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glDeleteFramebuffers(1,&WINDOW.FBO);
    WINDOW.FBO=0;
  }if(WINDOW.Tex!=0){
    glDeleteTextures(1,&WINDOW.Tex);
    WINDOW.Tex=0;
  }
  glDeleteProgram(texprogram);
  glDeleteProgram(triprogram);
  IMG_Quit();
  SDL_Quit();
}
