#pragma once
#include"Vec.h"

class Ray
{
public:
	vec3f o, dir;

	template <class T>
	Ray(vec3<T> _o, vec3<T> _d) :o(_o), dir(_d) {}
};