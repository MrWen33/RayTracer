#pragma once
#include<cmath>
#include"Material.h"
#include"Primitive.h"

class Sphere:public Primitive {
public:
	vec3f o;
	double r;

	Sphere(vec3f _o, double _r,const PhoneMaterial* _material)
		:o(_o), r(_r),Primitive(_material)
	{
	}

	void Intersect(const Ray& R,ClosestHitInfo& info) const//����������Բ�ཻ�ĵ�Ĳ���.��û���ཻ�򷵻�0
	{
		double eps = 0.0001;
		double a = R.dir.dot(R.dir);
		double b = 2 * R.dir.dot(R.o - o);
		double c = (R.o - o).dot(R.o - o) - r*r;

		double delta = b*b - 4 * a*c;
		if (delta < 0)return;
		else
		{
			double t;
			double t1, t2;
			double sqrtDelta = sqrt(delta);
			t2 = (-b + sqrtDelta) / (2 * a);
			t1 = (-b - sqrtDelta) / (2 * a);
			if (t1 > eps) t = t1;
			else if (t2 > eps) t = t2;
			else return;
			//����������Լ��㷨��
			vec3f hitPoint = R.o + R.dir*t;
			info.normal = (hitPoint - o).normalized();
			info.prim = this;
			info.min_t = t;
			return;
		}
	}
	AABB Bound() const {
		AABB bound;
		bound.MaxX = o.x + r;
		bound.MinX = o.x - r;
		bound.MaxY = o.y + r;
		bound.MinY = o.y - r;
		bound.MaxZ = o.z + r;
		bound.MinZ = o.z - r;
		bound.objs.push_back(this);
		return bound;
	}
};