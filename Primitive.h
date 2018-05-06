#pragma once
#include"Ray.h"
#include"Vec.h"
#include"Material.h"
#include"Boundary.h"
class AABB;
class ClosestHitInfo;

//һ������ĸ���
class Primitive {
public:
	const PhoneMaterial* material;
	Primitive(const PhoneMaterial* _material) :material(_material) 
	{
		if (material == NULL)material = &DIFFUSE_WHITE;
	}
	virtual double Intersect(const Ray& r, vec3f& normal) const = 0;//�����ཻ�ͷ���0
	virtual AABB Bound()const=0;
};

//�������Ԫ�ཻ����Ϣ
struct ClosestHitInfo {
	Primitive* prim = NULL;
	vec3f normal;
	double min_t = 1e9;
};