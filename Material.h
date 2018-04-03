#pragma once

class PhoneMaterial
{
public:
	vec3f diffuse, specular, emit;//ɢ����ɫ��������ɫ
	double  reflectiveness,alpha,refract;//����ȣ��߹��,��͸����,������
	PhoneMaterial()
		:diffuse(1), specular(0), emit(0), reflectiveness(0),alpha(1),refract(0) { }
	PhoneMaterial(vec3f diff, vec3f spec, vec3f _emit, double refl,double _alpha,double refr) 
		:diffuse(diff), specular(spec), emit(_emit), reflectiveness(refl),alpha(_alpha),refract(refr) {}
};
