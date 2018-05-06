#pragma once

class PhoneMaterial
{
public:
	vec3f diffuse, specular, emit;//散射颜色，反射颜色,发光度
	double  reflectiveness,alpha,refract;//高光度,不透明度,折射率
	PhoneMaterial()
		:diffuse(1), specular(0), emit(0), reflectiveness(0),alpha(1),refract(0) { }
	PhoneMaterial(vec3f diff, vec3f spec, vec3f _emit, double refl,double _alpha,double refr) 
		:diffuse(diff), specular(spec), emit(_emit), reflectiveness(refl),alpha(_alpha),refract(refr) {}
};

const PhoneMaterial DIFFUSE_RED(vec3f(.75, .25, .25), vec3f(.75, .25, .25), 0, 0, 1, 1.5);
const PhoneMaterial DIFFUSE_BLUE(vec3f(.25, .25, .75), vec3f(.25, .25, .75), 0, 0, 1, 1.5);
const PhoneMaterial DIFFUSE_WHITE(vec3f(1, 1, 1), vec3f(1, 1, 1), 0, 0, 1, 1.5);
const PhoneMaterial BLACK(vec3f(), vec3f(), 0, 0, 1, 1.5);
const PhoneMaterial LIGHT(vec3f(), vec3f(), 400, 0, 1, 1.5);
const PhoneMaterial MIRROR(vec3f(1, 1, 1), vec3f(1, 1, 1), 0, 1, 1, 1.5);
const PhoneMaterial REFR(vec3f(1, 1, 1), vec3f(1, 1, 1), 0, 1, 0, 1.5);