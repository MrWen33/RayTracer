#include "Render.h"

bool Render::Intersect(const Ray & r, double & t, int & id)//r为求交的光线，t为光线参数，id为与光线相交的物体id
{
	t = 1e9;
	for (int i = 0; i < scene.size(); ++i)
	{
		double distance = scene[i].Intersect(r);
		if (distance > 0 && distance < t)
		{
			t = distance;
			id = i;
		}
	}
	return t < 1e9;
}

vec3f Render::RayTracer(const Ray & r, int depth)
{
	if (depth > maxDepth) return vec3f();
	double t=0;
	int id=0;
	if (Intersect(r, t, id))
	{
		bool isInObj;//是否在物体内，为折射提供信息
		const Sphere& sphere = scene[id];
		vec3f hitPoint = r.o + r.dir*t;//光线击中点
		//计算法线并判断光线在物体内还是外
		vec3f normal = (sphere.o - hitPoint).normalized();
		if (normal.dot(r.dir) > 0)
		{
			normal = normal*-1;
			isInObj = false;
		}
		else isInObj = true;
		double eps = 0.0001;
		PhoneMaterial material = *(sphere.material);

		double diffPer = material.alpha*(1 - material.reflectiveness);
		double specPer = material.alpha*material.reflectiveness;
		double refrPer = 1 - material.alpha;
		if (depth < 1)//反射层数不多时将所有类型光线都计算出来
		{
			//return material.diffuse;
			vec3f diffDir, specDir, refrDir;
			diffDir = getDiffDir(r.dir, normal);
			specDir = getSpecDir(r.dir, normal);
			if (material.alpha > 1 - eps)
			{
				double refr = material.refract;
				if (isInObj) refr = 1 / refr;
				refrDir = getRefrDir(r.dir, normal, refr);
			}
			return vec3f(material.shininess)
				+ material.diffuse.mult(RayTracer(Ray(hitPoint, diffDir), depth + 1))*diffPer
				+ material.specular.mult(RayTracer(Ray(hitPoint, specDir), depth + 1))*specPer
					+ material.diffuse.mult(RayTracer(Ray(hitPoint, refrDir), depth + 1))*refrPer;
		}
		else//反射层数多时按反射比例随机计算
		{
			double randNum = rand() / (double)RAND_MAX;
			if (randNum < refrPer)
			{
				double refr = material.refract;
				if (isInObj) refr = 1 / refr;
				vec3f refrDir = getRefrDir(r.dir, normal, refr);
				return vec3f(material.shininess) + material.diffuse.mult(RayTracer(Ray(hitPoint, refrDir), depth + 1))*refrPer;
			}
			else if (randNum < refrPer + specPer)
			{
				vec3f specDir = getSpecDir(r.dir, normal);
				return vec3f(material.shininess) + material.specular.mult(RayTracer(Ray(hitPoint, specDir), depth + 1))*specPer;
			}
			else
			{
				vec3f diffDir = getDiffDir(r.dir, normal);
				return vec3f(material.shininess)
					+ material.diffuse.mult(RayTracer(Ray(hitPoint, diffDir), depth + 1))*diffPer;

			}
		}
	}
	else
	{
		return vec3f();
	}
}

vec3f Render::getDiffDir(vec3f dir, vec3f normal)
{
	vec3f newDir((double)rand() / (double)RAND_MAX * 2.f - 1, (double)rand() / (double)RAND_MAX * 2.f - 1, (double)rand() / (double)RAND_MAX * 2.f - 1);
	if (newDir.dot(normal) < 0) newDir = newDir*-1;
	newDir = newDir.normalized();
	return newDir;
}

vec3f Render::getSpecDir(vec3f dir, vec3f normal)
{
	vec3f tan = dir + normal*(-dir.dot(normal));
	vec3f newDir = dir*-1 + tan * 2;
	return newDir;
}

vec3f Render::getRefrDir(vec3f dir, vec3f normal, double refr)
{
	//折射
	double n = refr;//折射率
	double NdotD = normal.dot(dir);
	vec3f y = normal*NdotD;
	vec3f x = dir - y;
	double dirLenInv = 1 / dir.len();
	double cos1 = abs(NdotD *dirLenInv);
	double sin1 = x.len()*dirLenInv;
	double sin2 = sin1 / n;
	double cos2 = sqrt(1 - sin2*sin2);

	vec3f newDir = x.normalized()*sin2 + y.normalized()*cos2;
	return newDir;
}


void Render::render()
{
	//对于每个像素
#pragma omp parallel for schedule(dynamic, 1)
	for (int i = 0; i < H; ++i)
	{
		//命令行显示渲染进度
		std::cout << "\rgenerating:" << (int)((i) / (double)(H) * 10000) / 100.f << "%\t\t\t";
		for (int j = 0; j < W; ++j)
		{
			for (int sx = 0; sx < 2; ++sx)
				for (int sy = 0; sy < 2; ++sy)
				{
					for (int s = 0; s < Samples; ++s)
					{
						//计算视图平面上的坐标(加入了随机偏移量)
						double xbias = ((double)j + 0.25 + 0.5*sx + (rand() / (double)RAND_MAX  - 0.5)) / W;
						double ybias = 1 - ((double)i + 0.25 + 0.5*sy + (rand() / (double)RAND_MAX  - 0.5)) / H ;
						//根据坐标用照相机类计算投射光线
						Ray dir = cam.getRay(xbias, ybias);
						screen[i*W + j] = screen[i*W + j] + RayTracer(dir, 0) / (4.f*Samples);
					}
				}
		}
	}
}

double clamp(double n)
{
	if (n > 1)return 1;
	if (n < 0)return 0;
	return n;
}

int toInt(double n)
{
	//将rgb值由0-1转为0-255
	return 255 * clamp(n);
}

void Render::WriteToFile(std::string filename)
{
	std::ofstream out(filename);
	out << "P3\n";
	out << W << " " << H << " " << 255<<" ";
	for (int i = 0; i < W*H; ++i)
		out << toInt(screen[i].x) << " " << toInt(screen[i].y) << " " << toInt(screen[i].z)<<" ";
	out.close();
}
