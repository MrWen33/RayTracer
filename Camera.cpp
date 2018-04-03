#include "Camera.h"

Camera::Camera(vec3f _eye, vec3f _front, vec3f _up, double _fov)
{
	eye = _eye;
	front = _front.normalized();
	right = (front.cross(_up)).normalized();
	up = (right.cross(front)).normalized();
	fov = _fov;
	fovScale = 2*tan(fov*0.5*PI / 180);
}

