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
	Triangle(vec3f* p0, vec3f* p1, vec3f* p2,PhoneMaterial* _material):Primitive(_material)
	{
		p[0] = p0;
		p[1] = p1;
		p[2] = p2;
		P2P0 = *p0 - *p2;
		P2P1 = *p1 - *p2;
		normal = (P2P0.cross(P2P1*-1)).normalized();
	}

	double Intersect(const Ray& r,vec3f& Normal) const
	{
		//克莱默法则判断三角形与射线相交
		Normal = normal;
		double eps = 0.0001;
		vec3f O = r.o;
		vec3f D = r.dir;
		vec3f P2 = *p[2];
		vec3f P2O = O - *p[2];
		Mat33 Para(P2P0, P2P1, D);
		double InvParaDet = 1/Para.det();
		double u = Mat33(P2O, P2P1, D).det() * InvParaDet;
		if (u < 0 || u>1)return 0;
		double v= Mat33(P2P0, P2O, D).det() * InvParaDet;
		if (v < 0 || u+v>1)return 0;
		double t =- Mat33(P2P0, P2P1, P2O).det()*InvParaDet;
		if (t < eps)return 0;
		return t;
	}
};