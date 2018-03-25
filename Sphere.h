#pragma once
#include<cmath>
#include"Material.h"

class Sphere {
public:
	vec3f o;
	double r;
	PhoneMaterial* material;

	Sphere(vec3f _o, double _r, PhoneMaterial* _material)
		:o(_o), r(_r)
	{
		material = _material;
	}

	double Intersect(const Ray& R) const//返回射线与圆相交的点的参数.若没有相交则返回0
	{
		double eps = 0.0001;
		double a = R.dir.dot(R.dir);
		double b = 2 * R.dir.dot(R.o - o);
		double c = (R.o - o).dot(R.o - o) - r*r;

		double delta = b*b - 4 * a*c;
		if (delta < 0)return 0;
		else
		{
			double t1, t2;
			double sqrtDelta = sqrt(delta);
			t2 = (-b + sqrtDelta) / (2 * a);
			t1 = (-b - sqrtDelta) / (2 * a);
			if (t1 > eps) return t1;
			else if (t2 > eps) return t2;
			else return 0;
		}
	}
};