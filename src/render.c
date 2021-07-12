#include "render.h"
#include "file.h"

struct Window WINDOW;
SDL_Event e;
uint mainShader;
uint guiShader;
TTF_Font *font;

float frameTime;
int prevTime;
int currentTime;
float deltaTime;

int Render_init(char *title,struct Vec2 dim){
  if(SDL_Init(SDL_INIT_VIDEO)<0){
    printf("Failed to init SDL\n");
    return 0;
  }if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)<0){
    printf("Failed to init IMG\n");
    return 0;
  }if(TTF_Init()<0){
    printf("Failed to init TTF\n");
    return 0;
  }
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);

  WINDOW.window=SDL_CreateWindow(title,
      SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,dim.x,dim.y,
      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
  );
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

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);

  char* vertshader=LoadFile("shader/vertshader.glsl");
  char* fragshader=LoadFile("shader/fragshader.glsl");
  if(vertshader==NULL || fragshader==NULL){
    printf("File Error\n");
    Render_destroy();
    return 0;
  }
  mainShader=LoadShader(vertshader,fragshader);
  free(vertshader);free(fragshader);

  vertshader=LoadFile("shader/guivertex.glsl");
  fragshader=LoadFile("shader/guifragment.glsl");
  if(vertshader==NULL || fragshader==NULL){
    printf("File Error\n");
    Render_destroy();
    return 0;
  }
  guiShader=LoadShader(vertshader,fragshader);
  free(vertshader);free(fragshader);
  font=TTF_OpenFont("res/Arial.ttf",10);
  if(font==NULL){
    printf("Font Error\n");
    Render_destroy();
    return 0;
  }
  // printGLinfo();
  return 1;
}

void Render_update(void(*update)(float)){
  SDL_GL_MakeCurrent(WINDOW.window,WINDOW.context);
  //glBindFramebuffer(GL_FRAMEBUFFER,WINDOW.FBO);
  //glBindFramebuffer(GL_FRAMEBUFFER,WINDOW.FBO);
  //glViewport(0,0,1024,768);
  glClearColor(0.25,0.05,0.15,1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  prevTime=currentTime;
  currentTime=SDL_GetTicks();
  deltaTime=(currentTime-prevTime)/1000.0f;
  //glViewport(0,0,1024,768);
  glViewport(0,0,WINDOW.dim.x,WINDOW.dim.y);

  (*update)(1/deltaTime);


  //glViewport(0,0,WINDOW.dim.x,WINDOW.dim.y);
  SDL_GL_SwapWindow(WINDOW.window);
  SDL_Delay(10);
}

// struct Mesh UploadMesh(const struct Vertex *verts,const uint32_t *elems,int vertSize,int elemSize){
//   uint32_t VAO,VBO,EBO;
//
//   glGenVertexArrays(1,&VAO);
//   glBindVertexArray(VAO);
//
//   glGenBuffers(1,&VBO);
//   glBindBuffer(GL_ARRAY_BUFFER,VBO);
//   glBufferData(GL_ARRAY_BUFFER,vertSize*sizeof(struct Vertex),verts,GL_STATIC_DRAW);
//   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(struct Vertex),(const void*)offsetof(struct Vertex,Pos));
//   glEnableVertexAttribArray(0);
//
//   glGenBuffers(1,&EBO);
//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
//   glBufferData(GL_ELEMENT_ARRAY_BUFFER,elemSize*sizeof(uint32_t),elems,GL_STATIC_DRAW);
//
//   glBindVertexArray(0);
//   glDeleteBuffers(1,&VBO);
//   glDeleteBuffers(1,&EBO);
//
//   return ((struct Mesh){VAO,elemSize});
// }

struct Mesh ScreenMesh(){
  GLfloat verts[]={
    -1,1,-1,-1,
    1,1,1,-1
  };
  GLint elems[]={
    0,1,2,
    3,1,2
  };
  GLuint VAO,VBO,EBO;

  glGenBuffers(1,&VBO);

  glBindBuffer(GL_ARRAY_BUFFER,VBO);
  glBufferData(GL_ARRAY_BUFFER,8*sizeof(GLfloat),verts,GL_STATIC_DRAW);

  glGenVertexArrays(1,&VAO);
  glBindVertexArray(VAO);

  glEnableVertexAttribArray(0);

  glBindVertexBuffer(0,VBO,0,sizeof(GLfloat)*2);

  glVertexAttribFormat(0,2,GL_FLOAT,GL_FALSE,0);
  glVertexAttribBinding(0,0);

  glGenBuffers(1,&EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,6*sizeof(GLuint),elems,GL_STATIC_DRAW);

  return ((struct Mesh){VAO,(uint32_t)6});
}

struct Mesh UploadMesh(const GLfloat *verts,const GLfloat *colors,const GLuint *elems,int vertSize,int elemSize){
  GLuint VBO[2];
  GLuint VAO,EBO;

  glGenVertexArrays(1,&VAO);
  glBindVertexArray(VAO);

  glGenBuffers((colors==NULL)?1:2,VBO);

  glBindBuffer(GL_ARRAY_BUFFER,VBO[0]);
  glBufferData(GL_ARRAY_BUFFER,vertSize*sizeof(GLfloat),verts,GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glBindVertexBuffer(0,VBO[0],0,sizeof(GLfloat)*3);
  glVertexAttribFormat(0,3,GL_FLOAT,GL_FALSE,0);
  glVertexAttribBinding(0,0);

  if(colors!=NULL){
    glBindBuffer(GL_ARRAY_BUFFER,VBO[1]);
    glBufferData(GL_ARRAY_BUFFER,vertSize*sizeof(GLfloat),colors,GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glBindVertexBuffer(1,VBO[1],0,sizeof(GLfloat)*3);
    glVertexAttribFormat(1,3,GL_FLOAT,GL_FALSE,0);
    glVertexAttribBinding(1,1);
  }

  glGenBuffers(1,&EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,elemSize*sizeof(GLuint),elems,GL_STATIC_DRAW);

  return ((struct Mesh){VAO,(uint32_t)elemSize});
}

struct Mesh UploadMeshFull(const GLfloat *verts,const GLfloat *tex,const GLfloat *norm,int vertSize){
  GLuint VBO[3];
  GLuint VAO;

  glGenVertexArrays(1,&VAO);
  glBindVertexArray(VAO);

  glGenBuffers(3,VBO);

  glBindBuffer(GL_ARRAY_BUFFER,VBO[0]);
  glBufferData(GL_ARRAY_BUFFER,vertSize*sizeof(GLfloat)*3,verts,GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glBindVertexBuffer(0,VBO[0],0,sizeof(GLfloat)*3);
  glVertexAttribFormat(0,3,GL_FLOAT,GL_FALSE,0);
  glVertexAttribBinding(0,0);

  glBindBuffer(GL_ARRAY_BUFFER,VBO[1]);
  glBufferData(GL_ARRAY_BUFFER,vertSize*sizeof(GLfloat)*2,tex,GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glBindVertexBuffer(1,VBO[1],0,sizeof(GLfloat)*2);
  glVertexAttribFormat(1,2,GL_FLOAT,GL_FALSE,0);
  glVertexAttribBinding(1,1);

  glBindBuffer(GL_ARRAY_BUFFER,VBO[2]);
  glBufferData(GL_ARRAY_BUFFER,vertSize*sizeof(GLfloat)*3,norm,GL_STATIC_DRAW);
  glEnableVertexAttribArray(2);
  glBindVertexBuffer(2,VBO[2],0,sizeof(GLfloat)*3);
  glVertexAttribFormat(2,3,GL_FLOAT,GL_FALSE,0);
  glVertexAttribBinding(2,2);

  return ((struct Mesh){VAO,(uint32_t)vertSize});
}

struct Mesh LoadMesh(const char *file){
  DECLARE_LIST(GLfloat*,verts,8)
  DECLARE_LIST(GLfloat*,tex,8)
  DECLARE_LIST(GLfloat*,norm,8)
  // DECLARE_LIST(GLfloat*,colors,8)
  DECLARE_LIST(GLuint*,vertElems,8)
  DECLARE_LIST(GLuint*,texElems,8)
  DECLARE_LIST(GLuint*,normElems,8)
  char *data=LoadFile(file);
  int state=0;
  char *token;
  token=strtok(data,"/ \n");
  while(token!=NULL){
    // printf("%s\n",token);
    if(strcmp(token,"v")==0){
      state=4;
      // printf("%s\n",token);
    }else if(strcmp(token,"f")==0){
      state=1;
      // printf("%s\n",token);
    }else if(strcmp(token,"vt")==0){
      state=5;
      // printf("%s\n",token);
    }else if(strcmp(token,"vn")==0){
      state=6;
      // printf("%s\n",token);
    }else if(
      // strcmp(token,"vt")==0||
      // strcmp(token,"vn")==0||
      strcmp(token,"usemtl")==0||
      strcmp(token,"s")==0
    ){
      state=0;
    }else if(state==4){
      float f=atof(token);
      LIST_POPON(GLfloat*,verts,f)
      // f=(f+1)/5;
      // LIST_POPON(GLfloat*,colors,f)
      // printf("%s\n",token);
    }else if(state==5){
      LIST_POPON(GLfloat*,tex,atof(token))
    }else if(state==6){
      LIST_POPON(GLfloat*,norm,atof(token))
    }else if(state==3){
      LIST_POPON(GLuint*,normElems,atoi(token)-1)
      state=1;
    }else if(state==2){
      LIST_POPON(GLuint*,texElems,atoi(token)-1)
      state=3;
    }else if(state==1){
      LIST_POPON(GLuint*,vertElems,atoi(token)-1)
      // printf(" %i\n",atoi(token)-1);
      state=2;
    }
    token=strtok(NULL,"/ \n");
  }
  free(data);
  GLfloat *vertarr=malloc(sizeof(GLfloat)*GET_WIDTH(vertElems)*3);
  GLfloat *texarr=malloc(sizeof(GLfloat)*GET_WIDTH(vertElems)*2);
  GLfloat *normarr=malloc(sizeof(GLfloat)*GET_WIDTH(vertElems)*3);
  for(int i=0;i<GET_WIDTH(vertElems);i++){
    vertarr[i*3]=verts[vertElems[i]*3];
    vertarr[i*3+1]=verts[vertElems[i]*3+1];
    vertarr[i*3+2]=verts[vertElems[i]*3+2];
    texarr[i*2]=tex[texElems[i]*2];
    texarr[i*2+1]=tex[texElems[i]*2+1];
    normarr[i*3]=norm[normElems[i]*3];
    normarr[i*3+1]=norm[normElems[i]*3+1];
    normarr[i*3+2]=norm[normElems[i]*3+2];
    // printf("%i:%f,%f   %i\n",i,vertarr[i],texarr[i],vertElems[i]);
  }
  struct Mesh mesh=UploadMeshFull(vertarr,texarr,normarr,GET_WIDTH(vertElems));
  // struct Mesh mesh=UploadMesh(verts,NULL,elems,GET_WIDTH(verts),GET_WIDTH(elems));
  // for(int i=0;i<GET_WIDTH(verts);i++){
  //   printf("%f",verts[i]);
  //   if(i%3==2)
  //     printf("\n");
  //   else
  //     printf(",");
  // }printf("\n");
  free(vertarr);
  free(texarr);
  free(normarr);
  free((size_t*)verts-2);
  free((size_t*)tex-2);
  free((size_t*)norm-2);
  // free((size_t*)colors-2);
  free((size_t*)vertElems-2);
  free((size_t*)texElems-2);
  free((size_t*)normElems-2);
  return mesh;
}

void Draw(struct Mesh *details){
  foreach(struct Mesh d,details){
    glBindVertexArray(d.vao);
    glDrawElements(GL_TRIANGLES,d.numElements,GL_UNSIGNED_INT,NULL);
  }
  glBindVertexArray(0);
}

void DrawFull(struct Mesh *details){
  foreach(struct Mesh d,details){
    glBindVertexArray(d.vao);
    glDrawArrays(GL_TRIANGLES,0,d.numElements);
  }
  glBindVertexArray(0);
}

void UnloadMesh(struct Mesh *details){
  foreach(struct Mesh d,details){
    glDeleteBuffers(1,&d.vao);
  }
  free((size_t *)details-1);
}

GLuint LoadShader(const char* vertexshader, const char* fragmentshader){
	GLuint VertexShaderID=glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID=glCreateShader(GL_FRAGMENT_SHADER);

	GLint Result=GL_FALSE;
	int InfoLogLength;

	glShaderSource(VertexShaderID,1,&vertexshader,NULL);
	glCompileShader(VertexShaderID);

	glGetShaderiv(VertexShaderID,GL_COMPILE_STATUS,&Result);
	glGetShaderiv(VertexShaderID,GL_INFO_LOG_LENGTH,&InfoLogLength);
	if(InfoLogLength>0){
    GLchar infoLog[InfoLogLength];
    GLsizei length;
    glGetShaderInfoLog(VertexShaderID,InfoLogLength,&length,infoLog);
		printf("Vertex Shader Error:\n %s\n",infoLog);
	}

	glShaderSource(FragmentShaderID,1,&fragmentshader,NULL);
	glCompileShader(FragmentShaderID);

	glGetShaderiv(FragmentShaderID,GL_COMPILE_STATUS,&Result);
	glGetShaderiv(FragmentShaderID,GL_INFO_LOG_LENGTH,&InfoLogLength);
	if(InfoLogLength>0){
    GLchar infoLog[InfoLogLength];
    GLsizei length;
    glGetShaderInfoLog(VertexShaderID,InfoLogLength,&length,infoLog);
		printf("Fragment Shader Error:\n %s\n",infoLog);
	}

	GLuint ProgramID=glCreateProgram();
	glAttachShader(ProgramID,VertexShaderID);
	glAttachShader(ProgramID,FragmentShaderID);
	glLinkProgram(ProgramID);

	glGetProgramiv(ProgramID,GL_LINK_STATUS,&Result);
	glGetProgramiv(ProgramID,GL_INFO_LOG_LENGTH,&InfoLogLength);
	if(InfoLogLength>0){
		printf("Shader Program Error\n");
	}

	glDetachShader(ProgramID,VertexShaderID);
	glDetachShader(ProgramID,FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

GLuint Load_text(char *text,struct Vec3 c){
	GLuint texture;
  glGenTextures(1,&texture);
  glBindTexture(GL_TEXTURE_2D,texture);

  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

  SDL_Color color={c.x*255,c.y*255,c.z*255};
  SDL_Surface *surface=TTF_RenderText_Solid(font,text,color);
  if(surface==NULL){
    printf("Failed to load text\n");
    return texture;
  }
  surface=SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_ABGR8888,0);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,surface->w,surface->h,0,GL_RGBA,GL_UNSIGNED_BYTE,surface->pixels);
  glGenerateMipmap(GL_TEXTURE_2D);
  SDL_FreeSurface(surface);
  glBindTexture(GL_TEXTURE_2D,0);
	return texture;
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

void Bind_texture(uint unit,GLuint texture){
  if(unit<0 || unit>31) return;
  glActiveTexture(GL_TEXTURE0+unit);
  glBindTexture(GL_TEXTURE_2D,texture);
}

void printGLinfo(){
  const GLubyte* vendor=glGetString(GL_VENDOR);
  printf("GL VENDOR : %s\n",vendor);

  const GLubyte* renderer=glGetString(GL_RENDERER);
  printf("GL RENDERER : %s\n",renderer);

  const GLubyte* version=glGetString(GL_VERSION);
  printf("GL VERSION (string) : %s\n",version);

  GLint major,minor;
  glGetIntegerv(GL_MAJOR_VERSION,&major);
  glGetIntegerv(GL_MINOR_VERSION,&minor);
  printf("GL VERSION (int) : %d.%d\n",major,minor);

  const GLubyte* glslVersion=glGetString(GL_SHADING_LANGUAGE_VERSION);
  printf("GLSL VERSION : %s\n",glslVersion);
}

void Render_destroy(){
  if(font!=NULL){
    TTF_CloseFont(font);
  }if(WINDOW.context!=NULL){
    SDL_GL_DeleteContext(WINDOW.context);
    WINDOW.context=NULL;
  }if(WINDOW.window!=NULL){
    SDL_DestroyWindow(WINDOW.window);
    WINDOW.window=NULL;
  }if(WINDOW.Tex!=0){
    glDeleteTextures(1,&WINDOW.Tex);
    WINDOW.Tex=0;
  }
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}
