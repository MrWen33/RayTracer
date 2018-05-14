#include<iostream>
#include<ctime>
#include"Vec.h"
#include"Render.h"
#include"Camera.h"
#include"Sphere.h"
#include"Material.h"
#include"Triangle.h"
#include"ObjReader.h"
using namespace std;


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
	//Camera cam(vec3f(50, 52, 145.6), vec3f(0, -0.042612, -1), vec3f(0, 1, 0), 90);
	time_t begin = time(NULL);
	Camera cam(vec3f(0, 0, 30), vec3f(0, 0, -1), vec3f(0, 1, 0), 60);

	int samples = 16;
	int depth = 3;
	if (argc == 3)
	{
		sprintf(argv[1], "%d", samples);
		sprintf(argv[2], "%d", depth);
	}
	ObjReader oreader;
	oreader.loadFile("cat.obj",&REFR);
	oreader.scene.push_back(new Sphere(vec3f(0, 0, -20), 10, &DIFFUSE_BLUE));
	vector<Primitive*> lights;
	lights.push_back(new Sphere(vec3f(0, 15, 5), 1.5, &LIGHT));
	Render render(oreader.scene,lights,cam,samples,depth,1000,1000);
	//Render render(scene, cam, samples, depth, 1000, 1000);
	render.render(0);
	cout << "Rendering Time:" << time(NULL) - begin <<"s"<< endl;
	stringstream timeS;
	timeS << time(NULL) - begin;
	string Time;
	timeS >> Time;
	render.WriteToFile("image.ppm");
	//system("pause");
}