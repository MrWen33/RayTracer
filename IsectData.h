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

//�������Ԫ�ཻ����Ϣ
struct ClosestHitInfo {
	ClosestHitInfo();
	const Primitive* prim = NULL;
	vec3f normal;
	double min_t;
	static const double inf_t;
};//���ཻmin_tΪ1e9��0��primΪNULL
