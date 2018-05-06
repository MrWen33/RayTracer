#pragma once
#include"Vec.h"
#include"Ray.h"
#include"Sphere.h"
#include"Camera.h"
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<time.h>
#include<cmath>
#include"UtilFuncs.h"
#include"AccelerationStructure.h"


class Render {
private:
	std::vector<Primitive*>& scene;
	Camera cam;
	int W, H,Samples,maxDepth;
	vec3f* screen;
	const AccelStruct accelStruct;
	BVHNode* BVHRoot;

	bool Intersect(const Ray& r,double &t,int &id, vec3f& normal);
	bool Intersect(const Ray & r, double & t,const Primitive*& obj, vec3f& normal);
	vec3f RayTracer(const Ray& r, int depth);
	vec3f ExplicitRayTracer(const Ray& r, int depth,int E=1);
	vec3f TestTracer(const Ray& r);

	vec3f getDiffDir(vec3f normal);
	vec3f getSpecDir(vec3f dir, vec3f normal);
	vec3f getRefrDir(vec3f dir, vec3f normal, double refr,double* Kr=NULL,double* Kt=NULL);//¼ÆËã·ÆÄù¶û·´Éä
	Ray getShadowRay(const vec3f& hitPoint,const Sphere* LightSphere,double * omega=NULL);

public:
	Render(std::vector<Primitive*>& scene, Camera cam,int samples,int _maxDepth,int W=600,int H=600)
		:scene(scene),cam(cam),maxDepth(_maxDepth),W(W),H(H),accelStruct(scene)
	{	
		DEBUGPRINT("start build BVH");
		BVHRoot = new BVHNode(scene);
		BVHRoot->split(100);
		DEBUGPRINT("BVH build success");
		srand((unsigned)time(NULL));
		Samples = samples / 4;
		if (Samples < 1)Samples = 1;
		screen = new vec3f[W*H];
	}
	~Render() {
		delete[] screen;
	}
	
	void render(int mode);

	void WriteToFile(std::string filename);
	
};
