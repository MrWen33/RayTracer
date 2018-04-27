#include "Render.h"

bool Render::Intersect(const Ray & r, double & t, int & id, vec3f& normal)//rΪ�󽻵Ĺ��ߣ�tΪ���߲�����idΪ������ཻ������id
{
	t = 1e9;
	vec3f mayNormal;
	for (int i = 0; i < scene.size(); ++i)
	{
		double distance = scene[i]->Intersect(r, mayNormal);
		if (distance > 0 && distance < t)
		{
			t = distance;
			id = i;
			normal = mayNormal;
		}
	}
	return t < 1e9;
}

bool Render::Intersect(const Ray & r, double & t,const Primitive*& obj, vec3f& normal)//rΪ�󽻵Ĺ��ߣ�tΪ���߲�����objΪ������ཻ������ָ��
{
	ClosestHitInfo info;
	BVHRoot->Intersect(r,&info);
	t = info.min_t;
	obj = info.prim;
	normal = info.normal;
	return t<1e9;
}

vec3f Render::RayTracer(const Ray & r, int depth)
{
	if (depth > maxDepth) return vec3f();
	double t = 0;
	int id = 0;
	vec3f normal;
	if (Intersect(r, t, id, normal))
	{
		bool isInObj;//�Ƿ��������ڣ�Ϊ�����ṩ��Ϣ
		const Primitive& object = *scene[id];
		vec3f hitPoint = r.o + r.dir*t;//���߻��е�
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
		if (depth < 1)//�����������ʱ���������͹��߶��������
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
		else//���������ʱ����������������
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
	double t = 0;
	const Primitive* obj = NULL;
	vec3f normal;
	if (Intersect(r, t, obj, normal))
	{
		bool isInObj;//�Ƿ��������ڣ�Ϊ�����ṩ��Ϣ
		const Primitive& object = *obj;
		vec3f hitPoint = r.o + r.dir*t;//���߻��е�
		if (normal.dot(r.dir) > 0)
		{
			normal = normal*-1;
			isInObj = true;
		}
		
		else isInObj = false;

		double eps = 0.00001;

		const PhoneMaterial& material = *(object.material);

		double diffPer = material.alpha*(1 - material.reflectiveness);
		double specPer = material.alpha*material.reflectiveness;
		double refrPer = 1 - material.alpha;
		if (depth < 1)//�����������ʱ���������͹��߶��������
		{
			//return material.diffuse;
			vec3f specDir, refrDir;
			vec3f diffE;
			if (material.reflectiveness < 1 - eps)
			{
				//�жϵ��뷢����������Ƿ����ڵ���ȷ���Ƿ�����Ӱ
				for (int i = 0; i < scene.size(); ++i)
				{
					//�������з�����
					const Primitive* primitive = scene[i];
					if (primitive->material->emit.x <= 0&& primitive->material->emit.y <= 0 && primitive->material->emit.z <= 0 )continue;
					const Sphere* Light = dynamic_cast<const Sphere*>(primitive);
					double omega;
					Ray shadowRay = getShadowRay(hitPoint,Light,&omega);//���������Ӱ����
					int LightID;
					double t;
					vec3f nor;
					if (Intersect(shadowRay, t, LightID, nor) && LightID == i)//���������赲
					{
						double Cos = (normal.dot(shadowRay.dir));
						diffE = diffE + material.diffuse.mult(Light->material->emit*Cos*omega)*InvPI;
					}
				}
			}
			if(material.reflectiveness>eps)specDir = getSpecDir(r.dir, normal);
			if (material.alpha < 1 - eps)
			{
				double refr = material.refract;
				if (isInObj) refr = 1 / refr;
				refrDir = getRefrDir(r.dir, normal, refr);
			}
			return material.emit*E
				+ (diffE+material.diffuse.mult(ExplicitRayTracer(Ray(hitPoint,getDiffDir(normal)),depth+1,0)))*diffPer
				+ material.specular.mult(ExplicitRayTracer(Ray(hitPoint, specDir), depth + 1))*specPer
					+ material.diffuse.mult(ExplicitRayTracer(Ray(hitPoint, refrDir), depth + 1))*refrPer;
		}
		else//���������ʱ����������������
		{
			double randNum = rand01();
			if (randNum < refrPer)
			{
				double refr = material.refract;
				if (isInObj) refr = 1 / refr;
				vec3f refrDir = getRefrDir(r.dir, normal, refr);
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
				//�жϵ��뷢����������Ƿ����ڵ���ȷ���Ƿ�����Ӱ
				for (int i = 0; i < scene.size(); ++i)
				{
					//DEBUGPRINT(lights.size());
					//�������з�����
					const Primitive* primitive = scene[i];
					if (primitive->material->emit.x <= 0 && primitive->material->emit.y <= 0 && primitive->material->emit.z <= 0)continue;
					const Sphere* Light = dynamic_cast<const Sphere*>(primitive);
					double omega;
					Ray shadowRay = getShadowRay(hitPoint, Light, &omega);//���������Ӱ����
					int LightID;
					double t;
					vec3f nor;
					if (Intersect(shadowRay, t, LightID, nor) && LightID == i)//���������赲
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

vec3f Render::getRefrDir(vec3f dir, vec3f normal, double refr)
{
	//����
	double n = refr;//������
	double NdotD = normal.dot(dir);
	vec3f y = normal*NdotD;
	vec3f x = dir - y;
	double dirLenInv = 1 / dir.len();
	double sin1 = x.len()*dirLenInv;
	double sin2 = sin1 / n;
	double cos2 = sqrt(1 - sin2*sin2);

	vec3f newDir = x.normalized()*sin2 + y.normalized()*cos2;
	return newDir;
}

Ray Render::getShadowRay(const vec3f & hitPoint, const Sphere * LightSphere, double * omega)//
{
	assert(LightSphere != NULL);

	//��Բ��Դ�����������߽���ֱ������ϵ
	vec3f lightPos = LightSphere->o;
	vec3f w = (lightPos - hitPoint).normalized();
	vec3f tmp = (abs(w.x) > 0.1) ? vec3f(0, 1, 0) : vec3f(1, 0, 0);
	vec3f u = tmp.cross(w).normalized();
	vec3f v = w.cross(u).normalized();

	//���ѡȡ��Ӱ����
	double eps1 = rand01();
	double eps2 = rand01();
	double cos_a_max = sqrt(1 - LightSphere->r*LightSphere->r / (hitPoint - lightPos).dot(hitPoint - lightPos));
	double cos_a = 1 - eps1 + eps1*cos_a_max;
	double sin_a = sqrt(1 - cos_a*cos_a);
	double theta = 2 * PI*eps2;
	vec3f dir = w*cos_a + u*sin_a*cos(theta) + v*sin_a*sin(theta);
	dir = dir.normalized();
	vec3f no;
	if (omega != NULL) *omega = 2 * PI*(1 - cos_a_max);
	return Ray(hitPoint, dir);
}

void Render::render(int mode)
{
	//����ÿ������
#pragma omp parallel for schedule(dynamic, 1)
	for (int i = 0; i < H; ++i)
	{
		//��������ʾ��Ⱦ����
		fprintf(stderr, "\rRendering (%d spp) %5.2f%%", Samples * 4, 100.*i / (H - 1));
		for (int j = 0; j < W; ++j)
		{
			for (int sx = 0; sx < 2; ++sx)
				for (int sy = 0; sy < 2; ++sy)
				{
					vec3f r;
					for (int s = 0; s < Samples; ++s)
					{
						//������ͼƽ���ϵ�����(���������ƫ����)
						double xbias = ((double)j + 0.25 + 0.5*sx + (rand01() - 0.5)) / W;
						double ybias = 1 - ((double)i + 0.25 + 0.5*sy + (rand01() - 0.5)) / H;
						//��������������������Ͷ�����
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
		out << toInt(screen[i].x) << " " << toInt(screen[i].y) << " " << toInt(screen[i].z) << " ";
	out.close();
}