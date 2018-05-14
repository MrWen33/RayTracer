#pragma once
#include"Vec.h"
#include"Ray.h"
#include"Mat.h"
#include"Primitive.h"

class Triangle:public Primitive{//逆时针方向为正面
public:
	vec3f* p[3];
	vec3f normal;
	vec3f P2P0;
	vec3f P2P1;
	Triangle(vec3f* p0, vec3f* p1, vec3f* p2,const PhoneMaterial* _material=NULL):Primitive(_material)
	{
		p[0] = p0;
		p[1] = p1;
		p[2] = p2;
		P2P0 = *p0 - *p2;
		P2P1 = *p1 - *p2;
		normal = (P2P0.cross(P2P1*-1)).normalized();
	}

	Triangle(vec3f* p0, vec3f* p1, vec3f* p2, vec3f _normal=vec3f(),const PhoneMaterial* _material=NULL) :Primitive(_material)
	{
		p[0] = p0;
		p[1] = p1;
		p[2] = p2;
		P2P0 = *p0 - *p2;
		P2P1 = *p1 - *p2;
		if(_normal.len()==0)normal = (P2P0.cross(P2P1*-1)).normalized();
		else normal = _normal;
	}

	void Intersect(const Ray& r,ClosestHitInfo& info) const
	{
		//克莱默法则判断三角形与射线相交
		double eps = 0.0001;
		vec3f O = r.o;
		vec3f D = r.dir;
		vec3f P2 = *p[2];
		vec3f P2O = O - *p[2];
		Mat33 Para(P2P0, P2P1, D);
		double InvParaDet = 1/Para.det();
		double u = Mat33(P2O, P2P1, D).det() * InvParaDet;
		if (u < 0 || u>1)return;
		double v= Mat33(P2P0, P2O, D).det() * InvParaDet;
		if (v < 0 || u+v>1)return;
		double t =- Mat33(P2P0, P2P1, P2O).det()*InvParaDet;
		if (t < eps||t>info.min_t)return;
		info.normal = normal;
		info.prim = this;
		info.min_t = t;
		return;
	}

	AABB Bound() const
	{
		AABB bound;
		bound.reset(*(p[0]));
		bound.addPoint(*(p[1]));
		bound.addPoint(*(p[2]));
		bound.objs.push_back(this);
		return bound;
	}
};