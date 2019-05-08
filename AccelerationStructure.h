#pragma once
#include<vector>
#include<algorithm>
#include<memory>
#include"Primitive.h"
#include"Boundary.h"

class BVHNode {
	bool isLeaf;
	AABB bbox;
	BVHNode *L,*R;
	std::vector<Primitive*> primList;
public:

	BVHNode(std::vector<Primitive*>& scene)
	{
		isLeaf = true;
		primList = scene;
		bbox = primList[0]->Bound();
		for (int i = 0; i < primList.size(); ++i)
		{
			bbox.Union(primList[i]->Bound());
		}
		L = NULL;
		R = NULL;
	}
	BVHNode()
	{
	}
	double getDis(const Ray&r)
	{
		return bbox.getDis(r);
	}
	//判断射线是否与该节点相交
	bool isIntersect(const Ray& r)
	{
		if (!bbox.isIntersect(r))return false;
		if (isLeaf&&primList.size()>0)
		{
			ClosestHitInfo hitInfo;
			for (Primitive* prim : primList)
			{
				vec3f normal;
				prim->Intersect(r, hitInfo);
				if (hitInfo.min_t > 0&&hitInfo.min_t<1e9)
				{
					return true;
				}
			}
		}
		else
		{
			return L->isIntersect(r) || R->isIntersect(r);
		}
	}
	//计算射线与节点相交的信息
	void Intersect(const Ray& r,ClosestHitInfo* closest)
	{
		if (isLeaf&&primList.size()>0)
		{
			ClosestHitInfo info;
			for (Primitive* prim : primList)
			{
				prim->Intersect(r, info);
				double &t = info.min_t;
				if (t > 0 && t < closest->min_t)
				{
					closest->min_t = t;
					closest->normal = info.normal;
					closest->prim = info.prim;
				}
			}
		}
		else
		{
			double t1, t2;
			t1 = L->getDis(r);
			t2 = R->getDis(r);
			BVHNode* first, *second;
			if (t1 < t2)
			{
				first = L;
				second = R;
			}
			else
			{
				first = R;
				second = L;
			}
			//若与子包围盒能相交
			if (std::min(t1, t2) < 1e9)
			{
				first->Intersect(r, closest);
			}
			if (closest->min_t > std::max(t1, t2))
			{
				second->Intersect(r, closest);
			}
		}
	}

	size_t size()
	{
		return primList.size();
	}

	void split(int minSize)
	{
		if (size() > minSize)
		{
			int axis = 0;
			vec3f len = bbox.length();
			axis = len[0] > len[1] ? len[0] > len[2] ? 0 : 2 : len[1] > len[2] ? 1 : 2;
			sort(primList.begin(), primList.end(), [axis](Primitive* const & b1, Primitive* const &b2) {
				return b1->Bound().center()[axis] < b2->Bound().center()[axis];
			});
			int mid = 1;
			int bestMid = 1;
			double minSA = 1e9;
			for (; mid < primList.size(); ++mid) {
				AABB front = primList[0]->Bound(), back = primList[mid]->Bound();
				for (int i = 1; i < mid; ++i)
				{
					front.Union(primList[i]->Bound());
				}
				for (int i = mid; i < primList.size(); ++i)
				{
					back.Union(primList[i]->Bound());
				}
				double SA = front.surfaceArea()*(mid-1) + back.surfaceArea()*(primList.size()-mid);
				if (SA < minSA)
				{
					minSA = SA;
					bestMid = mid;
				}
			}
			//设置左右子节点
			L = new BVHNode;
			L->isLeaf = true;
			L->bbox = primList[0]->Bound();
			for (int i = 0; i < bestMid; ++i)
			{
				L->bbox.Union(primList[i]->Bound());
				L->primList.push_back(primList[i]);
			}
			R = new BVHNode;
			R->isLeaf = true;
			R->bbox = primList[bestMid]->Bound();
			for (int i = bestMid; i < primList.size(); ++i)
			{
				R->bbox.Union(primList[i]->Bound());
				R->primList.push_back(primList[i]);
			}
			isLeaf = false;
			primList.clear();
			L->split(minSize);
			R->split(minSize);
		}
	}
};

class BVH {
	BVHNode* root;
	BVH(const std::vector<Primitive*>& scene)
	{

	}
};

class AccelStruct {
private:
	std::vector<AABB> bounds;
	size_t size;
public:
	AccelStruct(const std::vector<Primitive*>& scene)
	{
		size = scene.size();
		for (size_t i = 0; i < scene.size(); i++)
		{
			bounds.push_back(scene[i]->Bound());
		}
		
	}
	~AccelStruct()
	{
	}
	double Intersect(const Ray& r,vec3f& Normal,const Primitive *&obj) const
	{
		double tClosest=1e9;
		for (int i = 0; i < size; ++i)
		{
			if (bounds[i].isIntersect(r))
			{
				ClosestHitInfo info;
				bounds[i].Intersect(r,info);
				double &t = info.min_t;
				if (t > 0 && t < tClosest)
				{
					tClosest = t;
					Normal = info.normal;
					obj = info.prim;
				}
			}
		}
		if (tClosest == 1e9) tClosest = 0;
		return tClosest;
	}
};