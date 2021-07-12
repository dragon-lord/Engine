#ifndef MATHS_H
#define MATHS_H

#include <math.h>

#define FIRST(A ...) A
#define VA_NUM_ARGS_IMPL(_1,_2,_3,_4,_5,N,...) N
#define VA_NUM_ARGS(...) VA_NUM_ARGS_IMPL(__VA_ARGS__,5,4,3,2,1)

#define GET_WIDTH(ARR)(*((size_t *)ARR-1))
#define GET_HEIGHT(ARR)(*((size_t *)ARR-2))
#define GET_DEPTH(ARR)(*((size_t *)ARR-3))
#define GET_VALUE(ARR,IND)(*((size_t *)ARR-IND))

#define ALLOC_ARR1(TYPE,VAR,SIZE)\
if(1){\
  size_t *raw=malloc(sizeof(size_t)+SIZE*sizeof(TYPE));\
  raw[0]=SIZE;\
  VAR=(TYPE)&(raw[1]);\
}
#define ALLOC_ARR2(TYPE,VAR,X,Y)\
if(1){\
  size_t *raw=malloc(2*sizeof(size_t)+X*Y*sizeof(TYPE));\
  raw[0]=X;\
  raw[1]=Y;\
  VAR=(TYPE)&(raw[2]);\
}
#define ALLOC_ARR3(TYPE,VAR,X,Y,Z)\
if(1){\
  size_t *raw=malloc(3*sizeof(size_t)+X*Y*Z*sizeof(TYPE));\
  raw[0]=X;\
  raw[1]=Y;\
  raw[2]=Z;\
  VAR=(TYPE)&(raw[3]);\
}

#define DECLARE_ARR(TYPE,VAR,...)TYPE VAR;ALLOC_ARR(TYPE,VAR,__VA_ARGS__)
#define ALLOC_ARR(TYPE,VAR,...) VA_NUM_ARGS_IMPL(__VA_ARGS__,5,4,ALLOC_ARR3,ALLOC_ARR2,ALLOC_ARR1)(TYPE,VAR,__VA_ARGS__)

#define DECLARE_LIST(TYPE,VAR,SIZE)TYPE VAR;ALLOC_LIST(TYPE,VAR,SIZE)
#define ALLOC_LIST(TYPE,VAR,SIZE)\
if(1){\
  size_t *raw=malloc(2*sizeof(size_t)+SIZE*sizeof(TYPE));\
  raw[0]=SIZE;\
  raw[1]=0;\
  VAR=(TYPE)&(raw[2]);\
}

#define LIST_POPON(TYPE,LIST,VAL)\
if(1){\
  int size=GET_WIDTH(LIST);\
  int actual=GET_HEIGHT(LIST);\
  if(size>=actual){\
    size_t *raw=(size_t*)LIST-2;\
    raw=realloc(raw,2*sizeof(size_t)+actual*2*sizeof(*LIST));\
    raw[0]=actual*2;\
    raw[1]=size;\
    LIST=(TYPE)&(raw[2]);\
  }\
  LIST[size]=VAL;\
  GET_WIDTH(LIST)=size+1;\
}
#define LIST_POPOFF(LIST)GET_WIDTH(LIST)-=1;

#define HEAP_PARENT(N)(((N)-1)/2)
#define HEAP_CHILD1(N)(2*(N)+1)
#define HEAP_CHILD2(N)(2*(N)+2)

#define HEAP_POPON(HEAP,VAL,CONDITION)\
if(1){\
  int pos=GET_WIDTH(HEAP);\
  HEAP[pos]=VAL;\
  SET_DIM(HEAP,pos+1);\
  while(1){\
    int parent=HEAP_PARENT(pos);\
    if(pos!=0 && CONDITION(HEAP[pos],HEAP[parent])){\
      HEAP[pos]=HEAP[parent];\
      HEAP[parent]=VAL;\
      pos=parent;\
    }else break;\
  }\
}
#define HEAP_POPOFF(HEAP,CONDITION)\
if(GET_WIDTH(HEAP)>0){\
  int pos=GET_WIDTH(HEAP)-1;\
  HEAP[0]=HEAP[pos];\
  SET_DIM(HEAP,pos);\
  pos=0;\
  while(GET_WIDTH(HEAP)>pos){\
    int child1=HEAP_CHILD1(pos);\
    int child2=HEAP_CHILD2(pos);\
    if(child1<GET_WIDTH(HEAP) && (CONDITION(HEAP[child1],HEAP[pos]) || CONDITION(HEAP[child2],HEAP[pos]))){\
      int temp=HEAP[pos];\
      if(child2>=GET_WIDTH(HEAP) || CONDITION(HEAP[child1],HEAP[child2])){\
        HEAP[pos]=HEAP[child1];\
        HEAP[child1]=temp;\
        pos=child1;\
      }else{\
        HEAP[pos]=HEAP[child2];\
        HEAP[child2]=temp;\
        pos=child2;\
      }\
    }else break;\
  }\
}

#define FOREACH(ITEM,ARR)\
for(int i=0,xyf=1;i<GET_WIDTH(ARR);xyf=!xyf,i++)\
for(ITEM=ARR[i];xyf;xyf=!xyf)
#define FOREACH1(INDEX,ITEM,ARR)\
for(int INDEX=0,xyf=1;INDEX<GET_WIDTH(ARR);xyf=!xyf,INDEX++)\
for(ITEM=ARR[i];xyf;xyf=!xyf)
#define FOREACH2(A,B,ITEM,ARR)\
for(int A=0;A<GET_WIDTH(ARR);A++)\
for(int B=0,xyf=1;B<GET_HEIGHT(ARR);xyf=!xyf,B++)\
for(ITEM=ARR[i];xyf;xyf=!xyf)
#define FOREACH3(A,B,C,ITEM,ARR)\
for(int A=0;A<GET_WIDTH(ARR);A++)\
for(int B=0;B<GET_HEIGHT(ARR);B++)\
for(int C=0,xyf=1;C<GET_HEIGHT(ARR);xyf=!xyf,C++)\
for(ITEM=ARR[i];xyf;xyf=!xyf)

#define foreach(...)VA_NUM_ARGS_IMPL(__VA_ARGS__,FOREACH3,FOREACH2,FOREACH1,FOREACH)(__VA_ARGS__)

#define VEC2_COMPONENT(V)V.x,V.y
#define VEC3_COMPONENT(V)V.x,V.y,V.z
#define VEC4_COMPONENT(V)V.w,V.x,V.y,V.z

#define VEC2_ISZERO(V)V.x==0&&V.y==0
#define VEC3_ISZERO(V)V.x==0&&V.y==0&&V.z==0
#define VEC4_ISZERO(V)V.w==0&&V.x==0&&V.y==0&&V.z==0

#define VEC2_EQUAL(A,B)A.x==B.x&&A.y==B.y
#define VEC3_EQUAL(A,B)A.x==B.x&&A.y==B.y&&A.z==B.z
#define VEC4_EQUAL(A,B)A.w==B.w&&A.x==B.x&&A.y==B.y&&A.z==B.z

#define VEC2_ZERO (struct Vec2){0,0}
#define VEC3_ZERO (struct Vec3){0,0,0}
#define VEC4_ZERO (struct Vec4){0,0,0,0}

#define VEC2_NEW(X,Y)     (struct Vec2){X,Y}
#define VEC3_NEW(X,Y,Z)   (struct Vec3){X,Y,Z}
#define VEC4_NEW(W,X,Y,Z) (struct Vec4){W,X,Y,Z}

#define new(...) VA_NUM_ARGS_IMPL(__VA_ARGS__,5,VEC4_NEW,VEC3_NEW,VEC2_NEW)(__VA_ARGS__)

#define M4(m,i,j) m.f[i+4*j]
#define Identity (struct Mat4){{\
  1,0,0,0,\
  0,1,0,0,\
  0,0,1,0,\
  0,0,0,1\
}}

struct Vec2{
	float x;
	float y;
};

struct Vec3{
	float x;
	float y;
	float z;
};

struct Vec4{
	float w;
	float x;
	float y;
	float z;
};

struct Mat4{
  float f[4*4];
};

struct Vec2 Vec2_new(float x,float y);
struct Vec3 Vec3_new(float x,float y,float z);
struct Vec4 Vec4_new(float w,float x,float y,float z);

struct Vec2 Vec2_norm(struct Vec2 v);
struct Vec2 Vec2_neg(struct Vec2 v);
struct Vec2 Vec2_cross(struct Vec2 v);
struct Vec2 Vec2_ortho2(struct Vec2 a,struct Vec2 b);
struct Vec2 Vec2_rot(struct Vec2 v,float theata);
float Vec2_length(struct Vec2 v);
float Vec2_dot(struct Vec2 a,struct Vec2 b);
float Vec2_angle(struct Vec2 a,struct Vec2 b);
float Vec2_det(struct Vec2 a,struct Vec2 b);

struct Vec3 Vec3_norm(struct Vec3 v);
struct Vec3 Vec3_neg(struct Vec3 v);
struct Vec3 Vec3_cross(struct Vec3 a,struct Vec3 b);
struct Vec3 Vec3_ortho2(struct Vec3 a,struct Vec3 b);
struct Vec3 Vec3_ortho3(struct Vec3 a,struct Vec3 b,struct Vec3 c);
struct Vec3 Vec3_rot(struct Vec3 a,struct Vec3 b,struct Vec3 c,float theata);
float Vec3_length(struct Vec3 v);
float Vec3_dot(struct Vec3 a,struct Vec3 b);
float Vec3_angle(struct Vec3 a,struct Vec3 b);
float Vec3_det(struct Vec3 a,struct Vec3 b,struct Vec3 c);

struct Vec4 Vec4_norm(struct Vec4 v);
struct Vec4 Vec4_neg(struct Vec4 v);
struct Vec4 Vec4_cross(struct Vec4 a,struct Vec4 b,struct Vec4 c);
struct Vec4 Vec4_ortho2(struct Vec4 a,struct Vec4 b);
struct Vec4 Vec4_ortho3(struct Vec4 a,struct Vec4 b,struct Vec4 c);
struct Vec4 Vec4_ortho4(struct Vec4 a,struct Vec4 b,struct Vec4 c,struct Vec4 d);
struct Vec4 Vec4_rot(struct Vec4 a,struct Vec4 b,struct Vec4 c,float theata);
float Vec4_length(struct Vec4 v);
float Vec4_dot(struct Vec4 a,struct Vec4 b);
float Vec4_angle(struct Vec4 a,struct Vec4 b);
float Vec4_det(struct Vec4 a,struct Vec4 b,struct Vec4 c,struct Vec4 d);

#define norm(X) _Generic(\
  (X),\
  struct Vec2: Vec2_norm,\
  struct Vec3: Vec3_norm,\
  struct Vec4: Vec4_norm\
)(X)
#define neg(X) _Generic(\
  (X),\
  struct Vec2: Vec2_neg,\
  struct Vec3: Vec3_neg,\
  struct Vec4: Vec4_neg\
)(X)
#define cross(X,...) _Generic(\
  (X),\
  struct Vec2: Vec2_cross,\
  struct Vec3: Vec3_cross,\
  struct Vec4: Vec4_cross\
)(X,__VA_ARGS__)
#define ortho(X,...) _Generic(\
  (X),\
  struct Vec2: Vec2_ortho2,\
  struct Vec3: VA_NUM_ARGS_IMPL(\
    __VA_ARGS__,5,4,3,\
    Vec3_ortho3,Vec3_ortho2\
  ),\
  struct Vec4: VA_NUM_ARGS_IMPL(\
    __VA_ARGS__,5,4,\
    Vec4_ortho4,Vec4_ortho3,Vec4_ortho2\
  )\
)(X,__VA_ARGS__)
#define rot(X,...) _Generic(\
  (X),\
  struct Vec2: Vec2_rot,\
  struct Vec3: Vec3_rot,\
  struct Vec4: Vec4_rot\
)(X,__VA_ARGS__)
#define len(X) _Generic(\
  (X),\
  struct Vec2: Vec2_length,\
  struct Vec3: Vec3_length,\
  struct Vec4: Vec4_length\
)(X)
#define dot(X,...) _Generic(\
  (X),\
  struct Vec2: Vec2_dot,\
  struct Vec3: Vec3_dot,\
  struct Vec4: Vec4_dot\
)(X,__VA_ARGS__)
#define angle(X,...) _Generic(\
  (X),\
  struct Vec2: Vec2_angle,\
  struct Vec3: Vec3_angle,\
  struct Vec4: Vec4_angle\
)(X,__VA_ARGS__)
#define det(X,...) _Generic(\
  (X),\
  struct Vec2: Vec2_det,\
  struct Vec3: Vec3_det,\
  struct Vec4: Vec4_det\
)(X,__VA_ARGS__)

struct Vec2 Vec2_addn(struct Vec2 v,float n);
struct Vec2 Vec2_addv(struct Vec2 a,struct Vec2 b);
struct Vec2 Vec2_subn(struct Vec2 v,float n);
struct Vec2 Vec2_subv(struct Vec2 a,struct Vec2 b);
struct Vec2 Vec2_muln(struct Vec2 v,float n);
struct Vec2 Vec2_mulv(struct Vec2 a,struct Vec2 b);
struct Vec2 Vec2_divn(struct Vec2 v,float n);
struct Vec2 Vec2_divv(struct Vec2 a,struct Vec2 b);

struct Vec3 Vec3_addn(struct Vec3 v,float n);
struct Vec3 Vec3_addv(struct Vec3 a,struct Vec3 b);
struct Vec3 Vec3_subn(struct Vec3 v,float n);
struct Vec3 Vec3_subv(struct Vec3 a,struct Vec3 b);
struct Vec3 Vec3_muln(struct Vec3 v,float n);
struct Vec3 Vec3_mulv(struct Vec3 a,struct Vec3 b);
struct Vec3 Vec3_divn(struct Vec3 v,float n);
struct Vec3 Vec3_divv(struct Vec3 a,struct Vec3 b);

struct Vec4 Vec4_addn(struct Vec4 v,float n);
struct Vec4 Vec4_addv(struct Vec4 a,struct Vec4 b);
struct Vec4 Vec4_subn(struct Vec4 v,float n);
struct Vec4 Vec4_subv(struct Vec4 a,struct Vec4 b);
struct Vec4 Vec4_muln(struct Vec4 v,float n);
struct Vec4 Vec4_mulv(struct Vec4 a,struct Vec4 b);
struct Vec4 Vec4_divn(struct Vec4 v,float n);
struct Vec4 Vec4_divv(struct Vec4 a,struct Vec4 b);

#define add(X,B) _Generic(\
  (X),\
  struct Vec2: _Generic(\
    (B),\
    float: Vec2_addn,\
    struct Vec2: Vec2_addv,\
    default: 0\
  ),\
  struct Vec3: _Generic(\
    (B),\
    float: Vec3_addn,\
    struct Vec3: Vec3_addv,\
    default: 0\
  ),\
  struct Vec4: _Generic(\
    (B),\
    float: Vec4_addn,\
    struct Vec4: Vec4_addv,\
    default: 0\
  )\
)(X,B)
#define sub(X,B) _Generic(\
  (X),\
  struct Vec2: _Generic(\
    (B),\
    float: Vec2_subn,\
    struct Vec2: Vec2_subv\
    default: 0\
  ),\
  struct Vec3: _Generic(\
    (B),\
    float: Vec3_subn,\
    struct Vec3: Vec3_subv\
    default: 0\
  ),\
  struct Vec4: _Generic(\
    (B),\
    float: Vec4_subn,\
    struct Vec4: Vec4_subv\
    default: 0\
  )\
)(X,B)
#define mul(X,B) _Generic(\
  (X),\
  struct Vec2: _Generic(\
    (B),\
    float: Vec2_muln,\
    struct Vec2: Vec2_mulv\
    default: 0\
  ),\
  struct Vec3: _Generic(\
    (B),\
    float: Vec3_muln,\
    struct Vec3: Vec3_mulv\
    default: 0\
  ),\
  struct Vec4: _Generic(\
    (B),\
    float: Vec4_muln,\
    struct Vec4: Vec4_mulv\
    default: 0\
  )\
)(X,B)
#define div(X,B) _Generic(\
  (X),\
  struct Vec2: _Generic(\
    (B),\
    float: Vec2_divn,\
    struct Vec2: Vec2_divv\
    default: 0\
  ),\
  struct Vec3: _Generic(\
    (B),\
    float: Vec3_divn,\
    struct Vec3: Vec3_divv\
    default: 0\
  ),\
  struct Vec4: _Generic(\
    (B),\
    float: Vec4_divn,\
    struct Vec4: Vec4_divv\
    default: 0\
  )\
)(X,B)

struct Mat4 Mat4_projection(float fov,float ratio,float near,float far);
struct Mat4 Mat4_lookat(struct Vec3 pos,struct Vec3 fwd,struct Vec3 up);

struct Vec4 Mat4_column(struct Mat4 m,int c);
struct Vec4 Mat4_row(struct Mat4 m,int r);

struct Mat4 Mat4_mul(struct Mat4 a,struct Mat4 b);

#endif //MATHS_H
