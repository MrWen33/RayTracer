#pragma once
#include"Ray.h"
#include"IsectData.h"
class canIntersect {
public:
	virtual void Intersect(const Ray& r, ClosestHitInfo& info)const = 0;
};