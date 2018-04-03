#include<iostream>
#include"Vec.h"
#include"Render.h"
#include"Camera.h"
#include"Sphere.h"
#include"Material.h"
#include"Triangle.h"
using namespace std;
PhoneMaterial DIFFUSE_RED(vec3f(.75, .25, .25), vec3f(.75, .25, .25), 0, 0, 1, 1.5);
PhoneMaterial DIFFUSE_BLUE(vec3f(.25, .25, .75), vec3f(.25, .25, .75), 0, 0, 1, 1.5);
PhoneMaterial DIFFUSE_WHITE(vec3f(1, 1, 1), vec3f(1, 1, 1), 0, 0, 1, 1.5);
PhoneMaterial BLACK(vec3f(), vec3f(), 0, 0, 1, 1.5);
PhoneMaterial LIGHT(vec3f(), vec3f(), 400, 0, 1, 1.5);
PhoneMaterial MIRROR(vec3f(1, 1, 1), vec3f(1, 1, 1), 0, 1, 1, 1.5);
PhoneMaterial REFR(vec3f(1, 1, 1), vec3f(1, 1, 1), 0, 1, 0.2, 1.5);

Sphere s1(vec3f(1e5 + 1, 40.8, 81.6), 1e5, &DIFFUSE_BLUE);
Sphere light(vec3f(50, 681.6 - .27, 81.6), 600, &LIGHT);//Lite */
vector<Primitive*> scene = {
	new Sphere(vec3f(1e5 + 1,40.8,81.6),1e5,&DIFFUSE_BLUE),//Left 
	new Sphere(vec3f(-1e5 + 99,40.8,81.6),1e5,&DIFFUSE_RED),//Rght 
	new Sphere(vec3f(50,40.8, 1e5),1e5,  &DIFFUSE_WHITE),//Back 
	new Sphere(vec3f(50,40.8,-1e5 + 170),1e5, &BLACK),//Frnt 
	new Sphere(vec3f(50, 1e5, 81.6),1e5,&DIFFUSE_WHITE),//Botm 
	new Sphere(vec3f(50,-1e5 + 81.6,81.6),1e5,&DIFFUSE_WHITE),//Top 
	new Sphere(vec3f(27,16.5,47),16.5,&MIRROR),//Mirr 
	new Sphere(vec3f(73,16.5,78),16.5,&REFR),//Glas 
	new Sphere(vec3f(50,81.6 - 16.5,81.6),1.5, &LIGHT) ,//Lite 
	//new Triangle(new vec3f(50,81,81.6),new vec3f(50,81,47),new vec3f(73,81,47),&LIGHT)
};

int main(int argc,char** argv)
{
	Camera cam(vec3f(50, 52, 145.6), vec3f(0, -0.042612, -1), vec3f(0, 1, 0), 90);
	int samples = 16;
	int depth = 4;
	if (argc == 3)
	{
		sprintf(argv[1], "%d", samples);
		sprintf(argv[2], "%d", depth);
	}
	Render render(scene,cam,samples,depth,1000,1000);
	render.render(true);
	render.WriteToFile("image.ppm");
}