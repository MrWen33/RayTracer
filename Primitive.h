#pragma once
#include"Ray.h"
#include"Vec.h"
#include"Material.h"
#include"Boundary.h"
#include"Interfaces.h"
class AABB;
class canIntersect;

//一切物体的父类
class Primitive :public canIntersect {
public:
	const PhoneMaterial* material;
	Primitive(const PhoneMaterial* _material) :material(_material)
	{
		if (material == NULL)material = &DIFFUSE_WHITE;
	}
	virtual void Intersect(const Ray& r, ClosestHitInfo& info) const = 0;
	virtual AABB Bound()const = 0;
};