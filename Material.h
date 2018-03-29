#pragma once

class PhoneMaterial
{
public:
	vec3f diffuse, specular;//ɢ����ɫ��������ɫ
	double  shininess, reflectiveness,alpha,refract;//����ȣ��߹��,��͸����,������
	PhoneMaterial()
		:diffuse(1), specular(0), shininess(0), reflectiveness(0),alpha(1),refract(0) { }
	PhoneMaterial(vec3f diff, vec3f spec, double shin, double refl,double _alpha,double refr) 
		:diffuse(diff), specular(spec), shininess(shin), reflectiveness(refl),alpha(_alpha),refract(refr) {}
};
