#include<iostream>
#include"Vec.h"
#include"Render.h"
#include"Camera.h"
#include"Sphere.h"
#include"Material.h"
using namespace std;
PhoneMaterial DIFFUSE_RED(vec3f(.75, .25, .25), vec3f(.75, .25, .25), 0, 0.1, 1, 0);
PhoneMaterial DIFFUSE_BLUE(vec3f(.25, .25, .75), vec3f(.25, .25, .75), 0, 0.1, 1, 0);
PhoneMaterial DIFFUSE_WHITE(vec3f(1, 1, 1), vec3f(1, 1,1), 0, 0.1, 1, 0);
PhoneMaterial BLACK(vec3f(), vec3f(), 0, 0.1, 1, 0);
PhoneMaterial LIGHT(vec3f(),vec3f(), 12, 0.1, 1, 0);
PhoneMaterial MIRROR(vec3f(1,1,1), vec3f(1,1,1), 0, 1, 1, 0);
PhoneMaterial REFR(vec3f(1, 1, 1), vec3f(1, 1, 1), 0, 1, 0.5, 0);

vector<Sphere> scene = {
	Sphere(vec3f(1e5 + 1,40.8,81.6),1e5,&DIFFUSE_BLUE),//Left 
	Sphere(vec3f(-1e5 + 99,40.8,81.6),1e5,&DIFFUSE_RED),//Rght 
	Sphere(vec3f(50,40.8, 1e5),1e5,  &DIFFUSE_WHITE),//Back 
	Sphere(vec3f(50,40.8,-1e5 + 170),1e5, &BLACK),//Frnt 
	Sphere(vec3f(50, 1e5, 81.6),1e5,&DIFFUSE_WHITE),//Botm 
	Sphere(vec3f(50,-1e5 + 81.6,81.6),1e5,&DIFFUSE_WHITE),//Top 
	Sphere(vec3f(27,16.5,47),16.5,&MIRROR),//Mirr 
	Sphere(vec3f(73,16.5,78),16.5,&REFR),//Glas 
	Sphere(vec3f(50,681.6 - .27,81.6),600, &LIGHT) //Lite 
};

int main()
{
	Camera cam(vec3f(50, 52, 135.6), vec3f(0, -0.042612, -1), vec3f(0, 1, 0), 90);
	Render render(scene,cam,100,4);
	render.render();
	render.WriteToFile("image.ppm");
}