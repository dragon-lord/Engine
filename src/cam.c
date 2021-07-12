#include "cam.h"

float mouseSensitivity=0.5;
float moveSpeed=10;

struct Cam Cam_new(struct Vec3 pos,struct Vec3 fwd,struct Vec3 up){
  struct Cam cam;
  cam.pos=pos;
  cam.fwd=fwd;
  cam.up=up;
  cam.rght=cross(up,fwd);
  return cam;
}

struct Cam Cam_update(struct Cam cam,float fps){
  struct Vec3 rght=Vec3_norm(Vec3_cross(cam.up,cam.fwd));
  struct Vec3 fwd=Vec3_norm(Vec3_cross(rght,cam.up));
  if(keys[0])
    cam.pos=Vec3_addv(cam.pos,Vec3_muln(fwd,moveSpeed/fps));
  if(keys[1])
    cam.pos=Vec3_subv(cam.pos,Vec3_muln(fwd,moveSpeed/fps));
  if(keys[2])
    cam.pos=Vec3_addv(cam.pos,Vec3_muln(rght,moveSpeed/fps));
  if(keys[3])
    cam.pos=Vec3_subv(cam.pos,Vec3_muln(rght,moveSpeed/fps));
  if(keys[4])
    cam.pos=Vec3_addv(cam.pos,Vec3_muln(cam.up,moveSpeed/fps));
  if(keys[5])
    cam.pos=Vec3_subv(cam.pos,Vec3_muln(cam.up,moveSpeed/fps));
  cam.fwd=Vec3_rot(cam.fwd,fwd,rght,mRel.x*(mouseSensitivity/fps));
  float angle=Vec3_angle(cam.fwd,cam.up);
  if((angle>0.1&&mRel.y<0) || (angle<M_PI-0.1&&mRel.y>0))
    cam.fwd=Vec3_rot(cam.fwd,cam.up,fwd,mRel.y*(mouseSensitivity/fps));
  return cam;
}
