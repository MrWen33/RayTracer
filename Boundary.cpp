#include "Boundary.h"
#include"Vec.h"

AABB::AABB()
{
	MaxX = MaxY = MaxZ = MinX = MinY = MinZ = 0;
}

bool AABB::isIntersect(const Ray & r) const
{
	double dis = (MinX - r.o.x) / r.dir.x;
	if (dis > 0)
	{
		double tarY = r.o.y + r.dir.y*dis;
		double tarZ = r.o.z + r.dir.z*dis;
		if (tarY > MinY&&tarY<MaxY&&tarZ>MinZ&&tarZ < MaxZ)return true;
	}
	dis = (MaxX - r.o.x) / r.dir.x;
	if (dis > 0)
	{
		double tarY = r.o.y + r.dir.y*dis;
		double tarZ = r.o.z + r.dir.z*dis;
		if (tarY > MinY&&tarY<MaxY&&tarZ>MinZ&&tarZ < MaxZ)return true;
	}
	dis = (MaxY - r.o.y) / r.dir.y;
	if (dis > 0)
	{
		double tarZ = r.o.z + r.dir.z*dis;
		double tarX = r.o.x + r.dir.x*dis;
		if (tarX > MinX&&tarX<MaxX&&tarZ>MinZ&&tarZ < MaxZ)return true;
	}
	dis = (MinY - r.o.y) / r.dir.y;
	if (dis > 0)
	{
		double tarZ = r.o.z + r.dir.z*dis;
		double tarX = r.o.x + r.dir.x*dis;
		if (tarX > MinX&&tarX<MaxX&&tarZ>MinZ&&tarZ < MaxZ)return true;
	}
	dis = (MinZ - r.o.z) / r.dir.z;
	if (dis > 0)
	{
		double tarY = r.o.y + r.dir.y*dis;
		double tarX = r.o.x + r.dir.x*dis;
		if (tarX > MinX&&tarX<MaxX&&tarY>MinY&&tarY < MaxY)return true;
	}
	dis = (MaxZ - r.o.z) / r.dir.z;
	if (dis > 0)
	{
		double tarY = r.o.y + r.dir.y*dis;
		double tarX = r.o.x + r.dir.x*dis;
		if (tarX > MinX&&tarX<MaxX&&tarY>MinY&&tarY < MaxY)return true;
	}
	return false;
}

void AABB::reset(const vec3f & p)
{
	MaxX = MinX = p.x;
	MaxY = MinY = p.y;
	MaxZ = MinZ = p.z;
}

void AABB::addPrimitive(const Primitive * p)
{
	objs.push_back(p);
	AABB newBound = p->Bound();
	if (objs.size() == 1)
	{
		MaxX = newBound.MaxX;
		MinX = newBound.MinX;
		MaxY = newBound.MaxY;
		MinY = newBound.MinY;
		MaxZ = newBound.MaxZ;
		MinZ = newBound.MinZ;
	}
	else
	{
		Union(newBound);
	}
}

void AABB::addPoint(vec3f p)
{
	MaxX = std::max(MaxX, p.x);
	MinX = std::min(MinX, p.x);
	MaxY = std::max(MaxY, p.y);
	MinY = std::min(MinY, p.y);
	MaxZ = std::max(MaxZ, p.z);
	MinZ = std::min(MinZ, p.z);
}

double AABB::getDis(const Ray & r) const//若在包围盒内则返回0，不相交则返回1e9
{
	if (isIn(r.o))return 0;
	double eps = 0.0001f;
	double t = 1e9;
	double dis = (MinX - r.o.x) / r.dir.x;
	if (dis > eps)
	{
		double tarY = r.o.y + r.dir.y*dis;
		double tarZ = r.o.z + r.dir.z*dis;
		if (tarY > MinY&&tarY<MaxY&&tarZ>MinZ&&tarZ < MaxZ)if (dis < t)t = dis;
	}
	dis = (MaxX - r.o.x) / r.dir.x;
	if (dis > eps)
	{
		double tarY = r.o.y + r.dir.y*dis;
		double tarZ = r.o.z + r.dir.z*dis;
		if (tarY > MinY&&tarY<MaxY&&tarZ>MinZ&&tarZ < MaxZ)if (dis < t)t = dis;
	}
	dis = (MaxY - r.o.y) / r.dir.y;
	if (dis > eps)
	{
		double tarZ = r.o.z + r.dir.z*dis;
		double tarX = r.o.x + r.dir.x*dis;
		if (tarX > MinX&&tarX<MaxX&&tarZ>MinZ&&tarZ < MaxZ)if (dis < t)t = dis;
	}
	dis = (MinY - r.o.y) / r.dir.y;
	if (dis > eps)
	{
		double tarZ = r.o.z + r.dir.z*dis;
		double tarX = r.o.x + r.dir.x*dis;
		if (tarX > MinX&&tarX<MaxX&&tarZ>MinZ&&tarZ < MaxZ)if (dis < t)t = dis;
	}
	dis = (MinZ - r.o.z) / r.dir.z;
	if (dis > eps)
	{
		double tarY = r.o.y + r.dir.y*dis;
		double tarX = r.o.x + r.dir.x*dis;
		if (tarX > MinX&&tarX<MaxX&&tarY>MinY&&tarY < MaxY)if (dis < t)t = dis;
	}
	dis = (MaxZ - r.o.z) / r.dir.z;
	if (dis > eps)
	{
		double tarY = r.o.y + r.dir.y*dis;
		double tarX = r.o.x + r.dir.x*dis;
		if (tarX > MinX&&tarX<MaxX&&tarY>MinY&&tarY < MaxY)if (dis < t)t = dis;
	}
	return t;
} 

double AABB::Intersect(const Ray & r, vec3f & Normal, const Primitive *& obj) const
{
	double t = 1e9;
	for (int i = 0; i < objs.size(); ++i)
	{
		vec3f N;
		double dis= objs[i]->Intersect(r, N);
		if (dis>0&&dis<t)
		{
			t = dis;
			Normal = N;
			obj = objs[i];
		}
	}
	if (t == 1e9)t = 0;
	return t;
}

void AABB::Union(const AABB & newBound)
{
	MaxX = std::max(MaxX, newBound.MaxX);
	MinX = std::min(MinX, newBound.MinX);
	MaxY = std::max(MaxY, newBound.MaxY);
	MinY = std::min(MinY, newBound.MinY);
	MaxZ = std::max(MaxZ, newBound.MaxZ);
	MinZ = std::min(MinZ, newBound.MinZ);
}

bool AABB::isIn(vec3f p) const
{
	if (p.x > MaxX)return false;
	if (p.x < MinX)return false;
	if (p.y > MaxY)return false;
	if (p.y < MinY)return false;
	if (p.z > MaxZ)return false;
	if (p.z < MinZ)return false;
	return true;
}

vec3f AABB::length() const
{
	return vec3f(MaxX-MinX,MaxY-MinY,MaxZ-MinZ);
}

vec3f AABB::center() const
{
	return vec3f((MinX + MaxX) / 2, (MinY + MaxY) / 2, (MinZ + MaxZ) / 2);
}

double AABB::surfaceArea() const
{
	vec3f l = length();
	return 2*(l.x*l.y+l.y*l.z+l.z*l.x);
}

AABB * AABB::buildBBox(const std::vector<Primitive*>& scene,int begin,int size)
{
	AABB* bound = new AABB();
	for (int i = begin; i < size; ++i)
	{
		bound->addPrimitive(scene[i]);
	}
	return bound;
}
