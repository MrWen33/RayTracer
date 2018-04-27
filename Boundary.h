#pragma once
#include"Primitive.h"

#include<queue>
#include"Ray.h"

class Primitive;

class AABB {
public:

	double MaxX,MaxY,MaxZ;
	double MinX,MinY,MinZ;
	std::vector<const Primitive *> objs;

	AABB();
	bool isIntersect(const Ray& r) const;
	void reset(const vec3f& p);
	void addPrimitive(const Primitive * p);
	void addPoint(vec3f p);
	double getDis(const Ray& r) const;
	double Intersect(const Ray& r, vec3f& Normal, const Primitive*& obj) const;
	void Union(const AABB& other);
	bool isIn(vec3f p) const;

	vec3f length() const;
	vec3f center() const;
	double surfaceArea() const;

	static AABB* buildBBox(const std::vector<Primitive* > & scene,int begin,int size);
};