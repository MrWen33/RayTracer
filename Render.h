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

class Render {
private:
	std::vector<Primitive*>& scene;
	Camera cam;
	int W, H,Samples,maxDepth;
	vec3f* screen;

	bool Intersect(const Ray& r,double &t,int &id, vec3f& normal);

	vec3f RayTracer(const Ray& r, int depth);

	vec3f getDiffDir(vec3f dir, vec3f normal);
	vec3f getSpecDir(vec3f dir, vec3f normal);
	vec3f getRefrDir(vec3f dir, vec3f normal, double refr);

public:
	Render(std::vector<Primitive*>& scene, Camera cam,int samples,int _maxDepth,int W=600,int H=600)
		:scene(scene),cam(cam),maxDepth(_maxDepth),W(W),H(H)
	{	
		srand((unsigned)time(NULL));
		Samples = samples / 4;
		if (Samples < 1)Samples = 1;
		screen = new vec3f[W*H];
	}
	~Render() {
		delete[] screen;
	}
	
	void render();

	void WriteToFile(std::string filename);
	
};