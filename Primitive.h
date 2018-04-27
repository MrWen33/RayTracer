#pragma once
#include"Ray.h"
#include"Vec.h"
#include"Material.h"
#include"Boundary.h"
class AABB;

//一切物体的父类
class Primitive {
public:
	const PhoneMaterial* material;
	Primitive(const PhoneMaterial* _material) :material(_material) 
	{
		if (material == NULL)material = &DIFFUSE_WHITE;
	}
	virtual double Intersect(const Ray& r, vec3f& normal) const = 0;
	virtual AABB Bound()const=0;
};