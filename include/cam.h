#ifndef CAM_H
#define CAM_H

#include "inputhandeler.h"

#define Cam_zero (struct Cam){{0,0,0},{0,0,1},{0,1,0},{1,0,0}}

struct Cam{
  struct Vec3 pos;
  struct Vec3 fwd;
  struct Vec3 up;
  struct Vec3 rght;
};

struct Cam Cam_new(struct Vec3 pos,struct Vec3 fwd,struct Vec3 up);
struct Cam Cam_update(struct Cam cam,float fps);

#endif //CAM_H
