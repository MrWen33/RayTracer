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
	Ray getRay(double x, double y);//x,y Ù”⁄0-1
};