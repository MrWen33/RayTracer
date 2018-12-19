#pragma once
#include"Vec.h"
#include"Ray.h"
//#include"Primitive.h"
class Primitive;

struct IsectData {
	vec3f Normal;
	vec3f Color;
	vec3f Emit;
};

//射线与基元相交的信息
struct ClosestHitInfo {
	ClosestHitInfo();
	const Primitive* prim = NULL;
	vec3f normal;
	double min_t;
	static const double inf_t;
};//不相交min_t为1e9或0，prim为NULL
