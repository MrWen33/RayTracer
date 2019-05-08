#include "Render.h"

bool Render::Intersect(const Ray & r, double & t, int & id, vec3f& normal)//r为求交的光线，t为光线参数，id为与光线相交的物体id
{
	t = 1e9;
	for (int i = 0; i < scene.size(); ++i)
	{
		ClosestHitInfo info;
		scene[i]->Intersect(r, info);
		double &distance = info.min_t;
		vec3f &mayNormal = info.normal;
		if (distance > 0 && distance < t)
		{
			t = distance;
			id = i;
			normal = mayNormal;
		}
	}
	return t < 1e9;
}

bool Render::Intersect(const Ray & r,ClosestHitInfo& info)//r为求交的光线，t为光线参数，obj为与光线相交的物体指针
{
	BVHRoot->Intersect(r,&info);
	for (int i = 0; i < lights.size(); ++i)
	{
		lights[i]->Intersect(r, info);
	}
	return info.min_t<1e9;
}

vec3f Render::RayTracer(const Ray & r, int depth)
{
	if (depth > maxDepth) return vec3f();
	double t = 0;
	int id = 0;
	vec3f normal;
	if (Intersect(r, t, id, normal))
	{
		bool isInObj;//是否在物体内，为折射提供信息
		const Primitive& object = *scene[id];
		vec3f hitPoint = r.o + r.dir*t;//光线击中点
		if (normal.dot(r.dir) > 0)
		{
			normal = normal*-1;
			isInObj = true;
		}
		else isInObj = false;
		double eps = 0.0001;
		PhoneMaterial material = *(object.material);

		double diffPer = material.alpha*(1 - material.reflectiveness);
		double specPer = material.alpha*material.reflectiveness;
		double refrPer = 1 - material.alpha;
		if (depth < 1)//反射层数不多时将所有类型光线都计算出来
		{
			//return material.diffuse;
			vec3f diffDir, specDir, refrDir;
			if (material.reflectiveness < 1 - eps)diffDir = getDiffDir(normal);
			if (material.reflectiveness > eps)specDir = getSpecDir(r.dir, normal);
			if (material.alpha < 1 - eps)
			{
				double refr = material.refract;
				if (isInObj) refr = 1 / refr;
				refrDir = getRefrDir(r.dir, normal, refr);
			}
			return vec3f(material.emit)
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
				return vec3f(material.emit) + material.diffuse.mult(RayTracer(Ray(hitPoint, refrDir), depth + 1))*refrPer;
			}
			else if (randNum < refrPer + specPer)
			{
				vec3f specDir = getSpecDir(r.dir, normal);
				return vec3f(material.emit) + material.specular.mult(RayTracer(Ray(hitPoint, specDir), depth + 1))*specPer;
			}
			else
			{
				vec3f diffDir = getDiffDir(normal);
				return vec3f(material.emit)
					+ material.diffuse.mult(RayTracer(Ray(hitPoint, diffDir), depth + 1))*diffPer;

			}
		}
	}
	else
	{
		return vec3f();
	}
}

vec3f Render::TestTracer(const Ray & r)
{
	const Primitive* obj = NULL;
	double t = 1e9;
	vec3f normal;
	ClosestHitInfo info;
	BVHRoot->Intersect(r, &info);
	//DEBUGPRINT(info.min_t);
	if (info.prim)
	{
		return info.prim->material->diffuse;
	}
	else return vec3f(0, 0, 0);

}

vec3f Render::ExplicitRayTracer(const Ray & r, int depth, int E)
{
	if (depth > maxDepth) return vec3f();
	ClosestHitInfo hitInfo;
	if (Intersect(r, hitInfo))
	{
		bool isInObj;//是否在物体内，为折射提供信息
		const Primitive& object = *hitInfo.prim;
		vec3f& normal = hitInfo.normal;
		vec3f hitPoint = r.o + r.dir*hitInfo.min_t;//光线击中点
		if (normal.dot(r.dir) > 0)
		{
			normal = normal*-1;
			isInObj = true;
		}
		
		else isInObj = false;

		double eps = 0.00001;

		const PhoneMaterial& material = *(object.material);
		
		//菲涅尔反射计算
		double refr = material.refract;
		if (isInObj) refr = 1.f / refr;
		double Kr, Kt;
		vec3f refrDir = getRefrDir(r.dir, normal, refr,&Kr,&Kt);

		double diffPer = material.alpha*(1 - material.reflectiveness);
		double specPer = material.alpha*material.reflectiveness;
		double refrPer = 1 - material.alpha;
		if (material.alpha == 0)
		{
			diffPer = 0;
			specPer = Kr;
			refrPer = Kt;
		}
		if (depth < 1)//反射层数不多时将所有类型光线都计算出来
		{
			//return material.diffuse;
			vec3f specDir;
			vec3f diffE;
			if (material.reflectiveness < 1 - eps)
			{
				//判断点与发光体的连线是否有遮挡来确定是否有阴影
				for (int i = 0; i < lights.size(); ++i)
				{
					//遍历所有发光体
					const Primitive* primitive = lights[i];
					if (primitive->material->emit.x <= 0&& primitive->material->emit.y <= 0 && primitive->material->emit.z <= 0 )continue;
					const Sphere* Light = dynamic_cast<const Sphere*>(primitive);
					double omega;
					Ray shadowRay = getShadowRay(hitPoint,Light,&omega);//随机产生阴影光线
					ClosestHitInfo SRHitInfo;
					if (Intersect(shadowRay, SRHitInfo) && SRHitInfo.prim == primitive)//若连线无阻挡
					{
						double Cos = (normal.dot(shadowRay.dir));
						diffE = diffE + material.diffuse.mult(Light->material->emit*Cos*omega)*InvPI;
					}
				}
			}
			if(material.reflectiveness>eps)specDir = getSpecDir(r.dir, normal);

			return material.emit*E
				+ (diffE+material.diffuse.mult(ExplicitRayTracer(Ray(hitPoint,getDiffDir(normal)),depth+1,0)))*diffPer
				+ material.specular.mult(ExplicitRayTracer(Ray(hitPoint, specDir), depth + 1))*specPer
					+ material.diffuse.mult(ExplicitRayTracer(Ray(hitPoint, refrDir), depth + 1))*refrPer;
		}
		else//反射层数多时按反射比例随机计算
		{
			double randNum = rand01();
			if (randNum < refrPer)
			{
				return material.emit + material.diffuse.mult(ExplicitRayTracer(Ray(hitPoint, refrDir), depth + 1))*refrPer;
			}
			else if (randNum < refrPer + specPer)
			{
				vec3f specDir = getSpecDir(r.dir, normal);
				return material.emit + material.specular.mult(ExplicitRayTracer(Ray(hitPoint, specDir), depth + 1))*specPer;
			}
			else
			{
				vec3f diffE;
				//判断点与发光体的连线是否有遮挡来确定是否有阴影
				for (int i = 0; i < lights.size(); ++i)
				{
					//DEBUGPRINT(lights.size());
					//遍历所有发光体
					const Primitive* primitive = lights[i];
					if (primitive->material->emit.x <= 0 && primitive->material->emit.y <= 0 && primitive->material->emit.z <= 0)continue;
					const Sphere* Light = dynamic_cast<const Sphere*>(primitive);
					double omega;
					Ray shadowRay = getShadowRay(hitPoint, Light, &omega);//随机产生阴影光线
					ClosestHitInfo SRHitInfo;
					if (Intersect(shadowRay, SRHitInfo) && SRHitInfo.prim == primitive)//若连线无阻挡
					{
						double Cos = (normal.dot(shadowRay.dir));
						diffE = diffE + material.diffuse.mult(Light->material->emit*Cos*omega)*InvPI;
					}
				}

				return material.emit*E + diffE
					+ material.diffuse.mult(ExplicitRayTracer(Ray(hitPoint, getDiffDir(normal)), depth + 1, 0));

			}
		}
	}
	else
	{
		return vec3f();
	}
}

vec3f Render::getDiffDir(vec3f normal)
{
	vec3f newDir(rand01() * 2.f - 1, rand01() * 2.f - 1, rand01() * 2.f - 1);
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

vec3f Render::getRefrDir(vec3f dir, vec3f normal, double refr,double* Kr,double* Kt)
{
	//折射
	double n = refr;//折射率
	double NdotD = normal.dot(dir);
	vec3f y = normal*NdotD;
	vec3f x = dir - y;
	double dirLenInv = 1 / dir.len();
	double sin1 = x.len()*dirLenInv;
	double sin2 = sin1 / n;
	double cos2 = sqrt(1 - sin2*sin2);
	if(Kr!=NULL&&Kt!=NULL)
	{
		double cos1 = sqrt(1.f - sin1 * sin1);
		double rParall = (refr*cos1 - cos2) / (refr*cos1 + cos2);
		double rVertical = (cos1 - refr * cos2) / (cos1 + refr * cos2);
		*Kr = 0.5*(rParall*rParall + rVertical * rVertical);
		*Kt = 1 - *Kr;
	}

	vec3f newDir = x.normalized()*sin2 + y.normalized()*cos2;
	return newDir;
}

Ray Render::getShadowRay(const vec3f & hitPoint, const Sphere * LightSphere, double * omega)//
{
	assert(LightSphere != NULL);
	vec3f lightPos = LightSphere->o;
	vec3f w = (lightPos - hitPoint).normalized();
	vec3f dir = w;
	dir = dir.normalized();
	if (omega != NULL) *omega = 1;
	return Ray(hitPoint, dir);
}

void Render::render(int mode)
{
	//对于每个像素
#pragma omp parallel for schedule(dynamic, 1)
	for (int i = 0; i < H; ++i)
	{
		//命令行显示渲染进度
		fprintf(stderr, "\rRendering (%d spp) %5.2f%%", Samples * 4, 100.*i / (H - 1));
		for (int j = 0; j < W; ++j)
		{
			for (int sx = 0; sx < 2; ++sx)
				for (int sy = 0; sy < 2; ++sy)
				{
					vec3f r;
					for (int s = 0; s < Samples; ++s)
					{
						//计算视图平面上的坐标(加入了随机偏移量)
						double xbias = ((double)j + 0.25 + 0.5*sx + (rand01() - 0.5)) / W;
						double ybias = 1 - ((double)i + 0.25 + 0.5*sy + (rand01() - 0.5)) / H;
						//根据坐标用照相机类计算投射光线
						Ray dir = cam.getRay(xbias, ybias);

						if (mode==1)r =r + RayTracer(dir, 0) / (double)Samples;
						else if(mode==2)r = r + TestTracer(dir) / (double)Samples;
						else r = r + ExplicitRayTracer(dir, 0) / (double)Samples;
					}
					screen[i*W + j] = screen[i*W + j] + vec3f(clamp(r.x), clamp(r.y), clamp(r.z))*.25f;
				}
		}
	}
}

void Render::WriteToFile(std::string filename)
{
	std::ofstream out(filename);
	out << "P3\n";
	out << W << " " << H << " " << 255 << " ";
	for (int i = 0; i < W*H; ++i)
		out << std::max(toInt(screen[i].x),0) << " " << std::max(0,toInt(screen[i].y)) << " " << std::max(0,toInt(screen[i].z)) << " ";
	out.close();
}