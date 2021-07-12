#include "maths.h"

struct Vec2 Vec2_new(float x,float y){
	struct Vec2 vec;
	vec.x=x;
	vec.y=y;
	return vec;
}

struct Vec3 Vec3_new(float x,float y,float z){
	struct Vec3 vec;
	vec.x=x;
	vec.y=y;
	vec.z=z;
	return vec;
}

struct Vec4 Vec4_new(float w,float x,float y,float z){
	struct Vec4 vec;
	vec.w=w;
	vec.x=x;
	vec.y=y;
	vec.z=z;
	return vec;
}

struct Vec2 Vec2_norm(struct Vec2 v){
	return Vec2_divn(v,Vec2_length(v));
}

struct Vec2 Vec2_neg(struct Vec2 v){
	return Vec2_subv(VEC2_NEW(0,0),v);
}

struct Vec2 Vec2_cross(struct Vec2 v){
	return VEC2_NEW(v.y,-v.x);
}

struct Vec2 Vec2_ortho2(struct Vec2 a,struct Vec2 b){
	b=Vec2_norm(b);
	return Vec2_subv(a,Vec2_muln(b,Vec2_dot(a,b)));
}

struct Vec2 Vec2_rot(struct Vec2 v,float theata){
	float cos=cosf(theata);
	float sin=sinf(theata);
	return VEC2_NEW(v.x*cos-v.y*sin,v.x*sin+v.y*cos);
}

float Vec2_length(struct Vec2 v){
	return sqrt(v.x*v.x+v.y*v.y);
}

float Vec2_dot(struct Vec2 a,struct Vec2 b){
	return (a.x*b.x+a.y*b.y);
}

float Vec2_angle(struct Vec2 a,struct Vec2 b){
	return acosf(Vec2_dot(a,b)/(Vec2_length(a)*Vec2_length(b)));
}

float Vec2_det(struct Vec2 a,struct Vec2 b){
	return (a.x*b.y)-(b.x*a.y);
}

struct Vec3 Vec3_norm(struct Vec3 v){
	if(Vec3_length(v)==0){
		return v;
	}
	return Vec3_divn(v,Vec3_length(v));
}

struct Vec3 Vec3_neg(struct Vec3 v){
	return Vec3_subv(Vec3_new(0,0,0),v);
}

struct Vec3 Vec3_cross(struct Vec3 a,struct Vec3 b){
  return Vec3_new(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x);
}

struct Vec3 Vec3_ortho2(struct Vec3 a,struct Vec3 b){
	//if(Vec3_equal(a,b) || Vec3_equal(b,Vec3_new(0,0,0)))
	//	return Vec3_new(0,0,0);
	b=Vec3_norm(b);
	return Vec3_subv(a,Vec3_muln(b,Vec3_dot(a,b)));
}

struct Vec3 Vec3_ortho3(struct Vec3 a,struct Vec3 b,struct Vec3 c){
	//if(Vec3_equal(a,b) || Vec3_equal(a,c) || Vec3_equal(b,c) || Vec3_equal(a,Vec3_new(0,0,0) || Vec3_equal(b,Vec3_new(0,0,0) || Vec3_equal(c,Vec3_new(0,0,0)))
	//	return Vec3_new(0,0,0);
	c=Vec3_norm(c);
	b=Vec3_norm(Vec3_ortho2(b,c));
	return Vec3_subv(a,Vec3_addv(Vec3_muln(b,Vec3_dot(a,b)),Vec3_muln(c,Vec3_dot(a,c))));
}

struct Vec3 Vec3_rot(struct Vec3 a,struct Vec3 b,struct Vec3 c,float theata){
	c=Vec3_norm(c);
	b=Vec3_norm(Vec3_ortho2(b,c));

	struct Vec2 vec=VEC2_NEW(Vec3_dot(a,b),Vec3_dot(a,c));
	a=Vec3_ortho3(a,b,c);
	vec=Vec2_rot(vec,theata);
	//printf("%f:%f,%f\n",theata,vec.x,vec.y);
	a=Vec3_addv(a,Vec3_addv(Vec3_muln(b,vec.x),Vec3_muln(c,vec.y)));
	//printf("%f,%f,%f\n",a.x,a.y,a.z);
	return a;
}

float Vec3_length(struct Vec3 v){
	return sqrtf(v.x*v.x+v.y*v.y+v.z*v.z);
}

float Vec3_dot(struct Vec3 a,struct Vec3 b){
	return (a.x*b.x+a.y*b.y+a.z*b.z);
}

float Vec3_angle(struct Vec3 a,struct Vec3 b){
	return acosf(Vec3_dot(a,b)/(Vec3_length(a)*Vec3_length(b)));
}

float Vec3_det(struct Vec3 a,struct Vec3 b,struct Vec3 c){
	return (a.x*b.y*c.z)+(b.x*c.y*a.z)+(c.x*a.y*b.z)-(c.x*b.y*a.z)-(b.x*a.y*c.z)-(a.x*c.y*b.z);
}

struct Vec4 Vec4_norm(struct Vec4 v){
	if(Vec4_length(v)==0){
		return v;
	}
	return Vec4_divn(v,Vec4_length(v));
}

struct Vec4 Vec4_neg(struct Vec4 v){
	return Vec4_subv(Vec4_new(0,0,0,0),v);
}

struct Vec4 Vec4_cross(struct Vec4 a,struct Vec4 b,struct Vec4 c){
  float fA=(b.w*c.x)-(b.x*c.w);
  float fB=(b.w*c.y)-(b.y*c.w);
  float fC=(b.w*c.z)-(b.z*c.w);
  float fD=(b.x*c.y)-(b.y*c.x);
  float fE=(b.x*c.z)-(b.z*c.x);
  float fF=(b.y*c.z)-(b.z*c.y);
  return Vec4_new(
    (a.x*fF)-(a.y*fE)+(a.z*fD),
    -(a.w*fF)+(a.y*fC)-(a.z*fB),
    (a.w*fE)-(a.x*fC)+(a.z*fA),
    -(a.w*fD)+(a.x*fB)-(a.y*fA)
  );
}

struct Vec4 Vec4_ortho2(struct Vec4 a,struct Vec4 b){
	b=Vec4_norm(b);
	return Vec4_subv(a,Vec4_muln(b,Vec4_dot(a,b)));
}

struct Vec4 Vec4_ortho3(struct Vec4 a,struct Vec4 b,struct Vec4 c){
	//if(Vec3_equal(a,b) || Vec3_equal(a,c) || Vec3_equal(b,c) || Vec3_equal(a,Vec3_new(0,0,0)))
	//	return Vec3_new(0,0,0);
	c=Vec4_norm(c);
	b=Vec4_norm(Vec4_ortho2(b,c));
	return Vec4_subv(a,Vec4_addv(Vec4_muln(b,Vec4_dot(a,b)),Vec4_muln(c,Vec4_dot(a,c))));
}

struct Vec4 Vec4_ortho4(struct Vec4 a,struct Vec4 b,struct Vec4 c,struct Vec4 d){
	//if(Vec3_equal(a,b) || Vec3_equal(a,c) || Vec3_equal(b,c) || Vec3_equal(a,Vec3_new(0,0,0)))
	//	return Vec3_new(0,0,0);
	d=Vec4_norm(d);
	c=Vec4_norm(Vec4_ortho2(c,d));
	b=Vec4_norm(Vec4_ortho3(b,c,d));
	return Vec4_subv(a,Vec4_addv(Vec4_muln(b,Vec4_dot(a,b)),Vec4_addv(Vec4_muln(c,Vec4_dot(a,c)),Vec4_muln(d,Vec4_dot(a,d)))));
}

struct Vec4 Vec4_rot(struct Vec4 a,struct Vec4 b,struct Vec4 c,float theata){
	c=Vec4_norm(c);
	b=Vec4_norm(Vec4_ortho2(b,c));

	struct Vec2 vec=VEC2_NEW(Vec4_dot(a,b),Vec4_dot(a,c));
	a=Vec4_ortho3(a,b,c);
	vec=Vec2_rot(vec,theata);
	//printf("%f:%f,%f\n",theata,vec.x,vec.y);
	a=Vec4_addv(a,Vec4_addv(Vec4_muln(b,vec.x),Vec4_muln(c,vec.y)));
	//printf("%f,%f,%f\n",a.x,a.y,a.z);
	return a;
}

float Vec4_length(struct Vec4 v){
	return sqrtf(v.w*v.w+v.x*v.x+v.y*v.y+v.z*v.z);
}

float Vec4_dot(struct Vec4 a,struct Vec4 b){
	return (a.w*b.w+a.x*b.x+a.y*b.y+a.z*b.z);
}

float Vec4_angle(struct Vec4 a,struct Vec4 b){
	return acosf(Vec4_dot(a,b)/(Vec4_length(a)*Vec4_length(b)));
}

float Vec4_det(struct Vec4 a,struct Vec4 b,struct Vec4 c,struct Vec4 d){
	return (a.w*b.x*c.y*d.z)+(b.w*c.x*d.y*a.z)+(c.w*d.x*a.y*b.z)+(d.w*a.x*b.y*c.z)
				-(d.w*c.x*b.y*a.z)-(c.w*b.x*a.y*d.z)-(b.w*a.x*d.y*c.z)-(a.w*d.x*c.y*b.z);
}

struct Vec2 Vec2_addn(struct Vec2 v,float n){
	v.x+=n;
	v.y+=n;
	return v;
}

struct Vec2 Vec2_addv(struct Vec2 a,struct Vec2 b){
	a.x+=b.x;
	a.y+=b.y;
	return a;
}

struct Vec2 Vec2_subn(struct Vec2 v,float n){
	v.x-=n;
	v.y-=n;
	return v;
}

struct Vec2 Vec2_subv(struct Vec2 a,struct Vec2 b){
	a.x-=b.x;
	a.y-=b.y;
	return a;
}

struct Vec2 Vec2_muln(struct Vec2 v,float n){
	v.x*=n;
	v.y*=n;
	return v;
}

struct Vec2 Vec2_mulv(struct Vec2 a,struct Vec2 b){
	a.x*=b.x;
	a.y*=b.y;
	return a;
}

struct Vec2 Vec2_divn(struct Vec2 v,float n){
	v.x/=n;
	v.y/=n;
	return v;
}

struct Vec2 Vec2_divv(struct Vec2 a,struct Vec2 b){
	a.x/=b.x;
	a.y/=b.y;
	return a;
}

struct Vec3 Vec3_addn(struct Vec3 v,float n){
	v.x+=n;
	v.y+=n;
	v.z+=n;
	return v;
}

struct Vec3 Vec3_addv(struct Vec3 a,struct Vec3 b){
	a.x+=b.x;
	a.y+=b.y;
	a.z+=b.z;
	return a;
}

struct Vec3 Vec3_subn(struct Vec3 v,float n){
	v.x-=n;
	v.y-=n;
	v.z-=n;
	return v;
}

struct Vec3 Vec3_subv(struct Vec3 a,struct Vec3 b){
	a.x-=b.x;
	a.y-=b.y;
	a.z-=b.z;
	return a;
}

struct Vec3 Vec3_muln(struct Vec3 v,float n){
	v.x*=n;
	v.y*=n;
	v.z*=n;
	return v;
}

struct Vec3 Vec3_mulv(struct Vec3 a,struct Vec3 b){
	a.x*=b.x;
	a.y*=b.y;
	a.z*=b.z;
	return a;
}

struct Vec3 Vec3_divn(struct Vec3 v,float n){
	v.x/=n;
	v.y/=n;
	v.z/=n;
	return v;
}

struct Vec3 Vec3_divv(struct Vec3 a,struct Vec3 b){
	a.x/=b.x;
	a.y/=b.y;
	a.z/=b.z;
	return a;
}

struct Vec4 Vec4_addn(struct Vec4 v,float n){
	v.w+=n;
	v.x+=n;
	v.y+=n;
	v.z+=n;
	return v;
}

struct Vec4 Vec4_addv(struct Vec4 a,struct Vec4 b){
	a.w+=b.w;
	a.x+=b.x;
	a.y+=b.y;
	a.z+=b.z;
	return a;
}

struct Vec4 Vec4_subn(struct Vec4 v,float n){
	v.w-=n;
	v.x-=n;
	v.y-=n;
	v.z-=n;
	return v;
}

struct Vec4 Vec4_subv(struct Vec4 a,struct Vec4 b){
	a.w-=b.w;
	a.x-=b.x;
	a.y-=b.y;
	a.z-=b.z;
	return a;
}

struct Vec4 Vec4_muln(struct Vec4 v,float n){
	v.w*=n;
	v.x*=n;
	v.y*=n;
	v.z*=n;
	return v;
}

struct Vec4 Vec4_mulv(struct Vec4 a,struct Vec4 b){
	a.w*=b.w;
	a.x*=b.x;
	a.y*=b.y;
	a.z*=b.z;
	return a;
}

struct Vec4 Vec4_divn(struct Vec4 v,float n){
	v.w/=n;
	v.x/=n;
	v.y/=n;
	v.z/=n;
	return v;
}

struct Vec4 Vec4_divv(struct Vec4 a,struct Vec4 b){
	a.w/=b.w;
	a.x/=b.x;
	a.y/=b.y;
	a.z/=b.z;
	return a;
}

struct Mat4 Mat4_projection(float fov,float ratio,float near,float far){
	float d2r=M_PI/180.0f;
  float ys=1.0f/tanf(d2r*fov/2.0f);
  float xs=ys/ratio;
  float nf=far-near;
  struct Mat4 r={0};
  M4(r,0,0)=xs;
  M4(r,1,1)=ys;
  M4(r,3,2)=(far+near)/nf;
  M4(r,2,3)=-1.0f;
  M4(r,2,2)=2*far*near/nf;
  return r;
}

struct Mat4 Mat4_lookat(struct Vec3 pos,struct Vec3 fwd,struct Vec3 up){
	struct Vec3 zaxis=Vec3_norm(fwd);
  struct Vec3 xaxis=Vec3_norm(Vec3_cross(Vec3_norm(up),zaxis));
  struct Vec3 yaxis=Vec3_cross(zaxis,xaxis);

  struct Mat4 trans=Identity;
  M4(trans,0,3)=-pos.x;
  M4(trans,1,3)=-pos.y;
  M4(trans,2,3)=-pos.z;
  struct Mat4 rot=Identity;
  M4(rot,0,0)=xaxis.x;
  M4(rot,0,1)=xaxis.y;
  M4(rot,0,2)=xaxis.z;
  M4(rot,1,0)=yaxis.x;
  M4(rot,1,1)=yaxis.y;
  M4(rot,1,2)=yaxis.z;
  M4(rot,2,0)=zaxis.x;
  M4(rot,2,1)=zaxis.y;
  M4(rot,2,2)=zaxis.z;
  return Mat4_mul(rot,trans);
}

struct Vec4 Mat4_column(struct Mat4 m,int c){
	return new(M4(m,0,c),M4(m,1,c),M4(m,2,c),M4(m,3,c));
}

struct Vec4 Mat4_row(struct Mat4 m,int r){
	return new(M4(m,r,0),M4(m,r,1),M4(m,r,2),M4(m,r,3));
}

struct Mat4 Mat4_mul(struct Mat4 a,struct Mat4 b){
	return (struct Mat4){{
		Vec4_dot(Mat4_row(a,0),Mat4_column(b,0)),Vec4_dot(Mat4_row(a,1),Mat4_column(b,0)),Vec4_dot(Mat4_row(a,2),Mat4_column(b,0)),Vec4_dot(Mat4_row(a,3),Mat4_column(b,0)),
		Vec4_dot(Mat4_row(a,0),Mat4_column(b,1)),Vec4_dot(Mat4_row(a,1),Mat4_column(b,1)),Vec4_dot(Mat4_row(a,2),Mat4_column(b,1)),Vec4_dot(Mat4_row(a,3),Mat4_column(b,1)),
		Vec4_dot(Mat4_row(a,0),Mat4_column(b,2)),Vec4_dot(Mat4_row(a,1),Mat4_column(b,2)),Vec4_dot(Mat4_row(a,2),Mat4_column(b,2)),Vec4_dot(Mat4_row(a,3),Mat4_column(b,2)),
		Vec4_dot(Mat4_row(a,0),Mat4_column(b,3)),Vec4_dot(Mat4_row(a,1),Mat4_column(b,3)),Vec4_dot(Mat4_row(a,2),Mat4_column(b,3)),Vec4_dot(Mat4_row(a,3),Mat4_column(b,3))
	}};
}
