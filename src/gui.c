#include "gui.h"

void Elem_draw(struct GuiElem elem){
  struct Vec2 parentPos=(elem.parent!=NULL)?elem.parent->pos:VEC2_ZERO;
  struct Vec2 pos1=Vec2_muln(Vec2_divv(Vec2_addv(elem.pos,parentPos),WINDOW.dim),2);
  struct Vec2 pos2=Vec2_muln(Vec2_divv(Vec2_addv(Vec2_addv(elem.pos,elem.dim),parentPos),WINDOW.dim),2);
  pos1.y=-pos1.y;
  pos2.y=-pos2.y;
  const GLfloat verts[]={
    pos1.x,pos2.y,.0f,
    pos2.x,pos2.y,.0f,
    pos1.x,pos1.y,.0f,
    pos2.x,pos1.y,.0f
  };
  GLuint elems[]={
    0,2,1,
    3,1,2
  };
  DECLARE_ARR(struct Mesh*,dets,1);
  dets[0]=UploadMesh(verts,NULL,elems,12,6);
  glUseProgram(guiShader);
  Draw(dets);
  UnloadMesh(dets);
  if(elem.children!=NULL)
  foreach(struct GuiElem *e,elem.children){
    if(e!=NULL)
    e->draw(*e);
  }
}

void EmptyElem_draw(struct GuiElem elem){
  if(elem.children!=NULL)
  foreach(struct GuiElem *e,elem.children){
    if(e!=NULL)
    e->draw(*e);
  }
}
void EmptyElem_update(struct GuiElem elem){
  if(elem.children!=NULL)
  foreach(struct GuiElem *e,elem.children){
    if(e!=NULL)
    e->update(*e);
  }
}

struct GuiElem *Container_new(struct Vec2 pos,struct Vec2 dim,struct GuiElem *children[],int nochildren){
  struct GuiElem *elem=malloc(sizeof(struct GuiElem));
  elem->pos=pos;
  elem->dim=dim;
  elem->draw=&EmptyElem_draw;
  elem->update=&EmptyElem_update;
  if(nochildren!=0){
    ALLOC_ARR(struct GuiElem**,elem->children,nochildren);
    for(int i=0;i<nochildren;i++){
      elem->children[i]=children[i];
      elem->children[i]->parent=elem;
    }
  }
  return elem;
}

struct GuiElem *Button_new(struct Vec2 pos,struct Vec2 dim,struct Vec3 color,char *text,void(*action)(),struct GuiElem *children[],int nochildren){
  struct GuiElem *elem=malloc(sizeof(struct GuiElem));
  elem->pos=pos;
  elem->dim=dim;
  elem->draw=&Button_draw;
  elem->update=&Button_update;
  elem->action=action;
  elem->color=color;
  // if(text!=NULL){
  //   elem->text=
  // }
  if(nochildren!=0){
    ALLOC_ARR(struct GuiElem**,elem->children,nochildren);
    for(int i=0;i<nochildren;i++){
      elem->children[i]=children[i];
      elem->children[i]->parent=elem;
    }
  }
  return elem;
}

void Button_draw(struct GuiElem elem){
  struct Vec2 parentPos=(elem.parent!=NULL)?elem.parent->pos:VEC2_ZERO;
  // struct Vec2 pos1=Vec2_muln(Vec2_divv(Vec2_addv(elem.pos,parentPos),WINDOW.dim),2);
  // struct Vec2 pos2=Vec2_muln(Vec2_divv(Vec2_addv(Vec2_addv(elem.pos,elem.dim),parentPos),WINDOW.dim),2);
  struct Vec2 aspect=new(WINDOW.dim.y/WINDOW.dim.x,1);
  struct Vec2 pos1=Vec2_mulv(Vec2_addv(elem.pos,parentPos),aspect);
  struct Vec2 pos2=Vec2_mulv(Vec2_addv(Vec2_addv(elem.pos,elem.dim),parentPos),aspect);
  // printf("%f,%f\n",VEC2_COMPONENT(pos1));
  const GLfloat verts[]={
    pos1.x,pos2.y,.0f,
    pos2.x,pos2.y,.0f,
    pos1.x,pos1.y,.0f,
    pos2.x,pos1.y,.0f
  };
  GLuint elems[]={
    0,1,2,
    3,2,1
  };
  DECLARE_ARR(struct Mesh*,dets,1);
  dets[0]=UploadMesh(verts,NULL,elems,12,6);
  glUseProgram(guiShader);
  int color=glGetUniformLocation(guiShader,"c");
  glUniform3fv(color,1,(const GLfloat*)&elem.color);
  Draw(dets);
  UnloadMesh(dets);
  if(elem.children!=NULL)
  foreach(struct GuiElem *e,elem.children){
    if(e!=NULL)
    e->draw(*e);
  }
}
int pressed=1;
void Button_update(struct GuiElem elem){
  struct Vec2 parentPos=(elem.parent!=NULL)?elem.parent->pos:VEC2_ZERO;
  struct Vec2 aspect=new(WINDOW.dim.y/WINDOW.dim.x,1);
  struct Vec2 pos1=Vec2_mulv(Vec2_divn(Vec2_addn(Vec2_mulv(Vec2_addv(elem.pos,parentPos),aspect),1),2),WINDOW.dim);
  struct Vec2 pos2=Vec2_mulv(Vec2_divn(Vec2_addn(Vec2_mulv(Vec2_addv(Vec2_addv(elem.pos,elem.dim),parentPos),aspect),1),2),WINDOW.dim);
  pos1.y=WINDOW.dim.y-pos1.y;
  pos2.y=WINDOW.dim.y-pos2.y;
  if(
    mButton[1]&&pressed&&
    mPos.x>pos1.x&&
    mPos.y<pos1.y&&
    mPos.x<pos2.x&&
    mPos.y>pos2.y
  ){
    elem.action();
    pressed=0;
  }else if(!mButton[1])pressed=1;
}

void Elem_destroy(struct GuiElem *elem){
  if(elem==NULL)
    return;
  if(elem->children!=NULL){
    for(int i=0;i<GET_WIDTH(elem->children);i++){
      if(elem->children[i]!=NULL)
      Elem_destroy(elem->children[i]);
    }
    free((size_t*)(elem->children)-1);
  }
  free(elem);
}
