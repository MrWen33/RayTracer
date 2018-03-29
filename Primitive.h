#pragma once
#include"Ray.h"
#include"Vec.h"
#include"Material.h"

//һ������ĸ���
class Primitive {
public:
	PhoneMaterial* material;
	Primitive(PhoneMaterial* _material) :material(_material) {}
	virtual double Intersect(const Ray& r, vec3f& normal) const = 0
	{
		DEBUGPRINT("WRONG");
	}
};