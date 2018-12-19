#pragma once
#include<iostream>
#include<cmath>
#include"Vec.h"
#include"Ray.h"
class Camera {
public:
	vec3f eye, front, up,right;
	double fov,fovScale;
	Camera(vec3f _eye, vec3f _front, vec3f _up, double _fov);
	inline Ray getRay(double x, double y);//x,y范围0-1
};


Ray Camera::getRay(double x, double y)
{
	vec3f dir = right*(x - 0.5)*fovScale + up*(y - 0.5)*fovScale + front;
	return Ray(eye, dir);
}