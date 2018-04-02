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

class Render {
private:
	std::vector<Primitive*>& scene;
	std::vector<Primitive*> lights;
	Camera cam;
	int W, H,Samples,maxDepth;
	vec3f* screen;

	bool Intersect(const Ray& r,double &t,int &id, vec3f& normal);

	vec3f RayTracer(const Ray& r, int depth);
	vec3f ExplicitRayTracer(const Ray& r, int depth,int E=1);

	vec3f getDiffDir(vec3f normal);
	vec3f getSpecDir(vec3f dir, vec3f normal);
	vec3f getRefrDir(vec3f dir, vec3f normal, double refr);
	Ray getShadowRay(const vec3f& hitPoint,const Sphere* LightSphere);

public:
	Render(std::vector<Primitive*>& scene, Camera cam,int samples,int _maxDepth,int W=600,int H=600)
		:scene(scene),cam(cam),maxDepth(_maxDepth),W(W),H(H)
	{	
		srand((unsigned)time(NULL));
		Samples = samples / 4;
		if (Samples < 1)Samples = 1;
		screen = new vec3f[W*H];
		for (int i = 0; i < scene.size(); ++i)
		{
			if (scene[i]->material->shininess <= 0)continue;
			lights.push_back(scene[i]);
		}
	}
	~Render() {
		delete[] screen;
	}
	
	void render(bool isExplict);

	void WriteToFile(std::string filename);
	
};