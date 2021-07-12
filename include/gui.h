#ifndef GUI_H
#define GUI_H

#include "render.h"

struct GuiElem{
  struct GuiElem *parent;
  struct GuiElem **children;
  struct Vec2 pos;
  struct Vec2 dim;
  void(*draw)(struct GuiElem);
  void(*update)(struct GuiElem);
  void(*action)();
  struct Vec3 color;
  uint16_t state;
  char *text;
};
void Elem_draw(struct GuiElem elem);
void EmptyElem_draw(struct GuiElem elem);
void EmptyElem_update(struct GuiElem elem);

struct GuiElem *Container_new(struct Vec2 pos,struct Vec2 dim,struct GuiElem *children[],int nochildren);

struct GuiElem *Button_new(struct Vec2 pos,struct Vec2 dim,struct Vec3 color,char *text,void(*action)(),struct GuiElem *children[],int nochildren);
void Button_draw(struct GuiElem elem);
void Button_update(struct GuiElem elem);

void Elem_destroy(struct GuiElem *elem);

#endif //GUI_H
