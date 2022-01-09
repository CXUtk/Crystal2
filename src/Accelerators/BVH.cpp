#include "BVH.h"
#include <Core/Geometry.h>
#include <Core/Entities.h>
#include <algorithm>
#include <Core/SurfaceInteraction.h>


enum class SplitMethod
{
	EQUAL_COUNTS,
	SAH
};
constexpr int MAX_OBJ_IN_NODE = 2;
constexpr SplitMethod SLILT_METHOD = SplitMethod::EQUAL_COUNTS;
constexpr float TRAV_COST = 0.5f;
constexpr float INTERSECT_COST = 1.0f;

struct BVHNode
{
	BoundingBox bound;
	int entitiesStartOffset, count;
	int ch[2];
	int splitAxis;
};


BVH::BVH() : _nodes(nullptr), _tot(0), _root(0)
{}

BVH::~BVH()
{
	if (_nodes)
		delete[] _nodes;
}

void BVH::Build(const std::vector<const crystal::IIntersectable*>& objects)
{
	for (auto& obj : objects)
	{
		_objects.push_back(obj);
	}
	_nodes = new BVHNode[objects.size() * 5];
	_tot = 0;
	_build(_root, 0, objects.size() - 1);
}

bool BVH::Intersect(const Ray& ray, SurfaceInteraction* info, float tMin, float tMax) const
{
	if (!RayBoxTest(ray, _nodes[_root].bound, tMin, tMax)) return false;
	return _intersect(_root, ray, info, tMin, tMax);
}

bool BVH::IntersectTest(const Ray& ray, const crystal::IIntersectable* ignoreShape, 
	float tMin, float tMax) const
{
	if (!RayBoxTest(ray, _nodes[_root].bound, tMin, tMax)) return false;
	return _intersectP(_root, ray, ignoreShape, tMin, tMax);
}

void BVH::_build(int& p, int l, int r)
{
	if (r - l + 1 <= MAX_OBJ_IN_NODE)
	{
		createLeaf(p, l, r);
		return;
	}
	// 获取[l, r]的碰撞箱
	BoundingBox box = _objects[l]->GetBoundingBox();
	for (int i = l + 1; i <= r; i++)
	{
		box = box.Union(_objects[i]->GetBoundingBox());
	}

	int splitAxis, splitPos;
	if constexpr (SLILT_METHOD == SplitMethod::EQUAL_COUNTS)
	{
		splitAxis = splitByEqualCount(l, r, box, splitPos);
	}
	else
	{
		if (!splitBySAH(l, r, box, splitAxis, splitPos))
		{
			createLeaf(p, l, r);
			return;
		}
	}

	// 按照排序结果从中间分割物体们
	p = newNode(0, 0, splitAxis, box);
	_build(_nodes[p].ch[0], l, splitPos);
	_build(_nodes[p].ch[1], splitPos + 1, r);
}

int BVH::newNode(int offset, int cnt, int splitAxis, const BoundingBox& box)
{
	_tot++;
	_nodes[_tot].entitiesStartOffset = offset;
	_nodes[_tot].bound = box;
	_nodes[_tot].count = cnt;
	_nodes[_tot].splitAxis = splitAxis;
	return _tot;
}

bool BVH::_intersect(int p, const Ray& ray, SurfaceInteraction* info, float tMin, float tMax) const
{
	int objCnt = _nodes[p].count;
	bool hit = false;
	// 如果是叶子节点就是暴力判定一下
	if (objCnt != 0)
	{
		SurfaceInteraction isec;
		const crystal::IIntersectable* const* startP = &_objects[_nodes[p].entitiesStartOffset];
		for (int i = 0; i < objCnt; i++)
		{
			float t1 = tMin, t2 = std::min(tMax, info->GetDistance());
			if (!RayBoxTest(ray, startP[i]->GetBoundingBox(), t1, t2)) continue;
			if (!startP[i]->Intersect(ray, &isec)) continue;
			auto dis = isec.GetDistance();
			if (dis < std::numeric_limits<float>::infinity())
			{
				hit = true;
			}
			if (dis < info->GetDistance())
			{
				isec.SetHitEntity(dynamic_cast<const crystal::Entity*>(startP[i]));

				*info = std::move(isec);
			}
		}
		return hit;
	}

	int initDir = ray.dir[_nodes[p].splitAxis] > 0 ? 0 : 1;

	float t1 = tMin, t2 = std::min(tMax, info->GetDistance());
	if (RayBoxTest(ray, _nodes[_nodes[p].ch[initDir]].bound, t1, t2))
	{
		hit |= _intersect(_nodes[p].ch[initDir], ray, info, t1, t2);
	}

	t1 = tMin, t2 = std::min(tMax, info->GetDistance());
	if (RayBoxTest(ray, _nodes[_nodes[p].ch[!initDir]].bound, t1, t2))
	{
		hit |= _intersect(_nodes[p].ch[!initDir], ray, info, t1, t2);
	}

	return hit;
}

bool BVH::_intersectP(int p, const Ray& ray, const crystal::IIntersectable* ignoreShape, 
	float tMin, float tMax) const
{
	int objCnt = _nodes[p].count;
	// 如果是叶子节点就是暴力判定一下
	if (objCnt != 0)
	{
		const crystal::IIntersectable* const* startP = &_objects[_nodes[p].entitiesStartOffset];
		for (int i = 0; i < objCnt; i++)
		{
			if (startP[i] == ignoreShape) continue;
			float t1 = tMin, t2 = tMax;
			if (!RayBoxTest(ray, startP[i]->GetBoundingBox(), t1, t2)) continue;
			if (!startP[i]->IntersectTest(ray, ignoreShape, t1, t2)) continue;
			return true;
		}
		return false;
	}

	bool hit = false;
	float t1 = tMin, t2 = tMax;
	if (RayBoxTest(ray, _nodes[_nodes[p].ch[0]].bound, t1, t2))
	{
		hit |= _intersectP(_nodes[p].ch[0], ray, ignoreShape, t1, t2);
	}
	if (hit) return true;
	if (RayBoxTest(ray, _nodes[_nodes[p].ch[1]].bound, tMin, tMax))
	{
		hit |= _intersectP(_nodes[p].ch[1], ray, ignoreShape, tMin, tMax);
	}
	return hit;
}

int BVH::splitByEqualCount(int l, int r, const BoundingBox& box, int& splitPos)
{
	int splitAxis = box.MaxExtent();
	// 按照某种方式分割物体，并且排序
	auto cmp = [splitAxis](const crystal::IIntersectable* a, const crystal::IIntersectable* b) {
		return a->GetBoundingBox().GetCenter()[splitAxis] < b->GetBoundingBox().GetCenter()[splitAxis];
	};
	splitPos = l + (r - l) / 2;
	std::nth_element(_objects.begin() + l, _objects.begin() + splitPos, _objects.begin() + r + 1, cmp);
	return splitAxis;
}

bool BVH::splitBySAH(int l, int r, const BoundingBox& box, int& splitAxis, int& splitPos)
{
	splitAxis = 0, splitPos = l;
	float minCost = std::numeric_limits<float>::infinity();
	const crystal::IIntersectable* const* startP = &_objects[l];
	float totArea = box.SurfaceArea();
	float* sufArea = new float[r - l + 2];
	for (int i = 0; i < 3; i++)
	{
		// 按照中点坐标排序
		auto cmp = [i](const crystal::IIntersectable* a, const crystal::IIntersectable* b) {
			return a->GetBoundingBox().GetCenter()[i] < b->GetBoundingBox().GetCenter()[i];
		};
		std::sort(_objects.begin() + l, _objects.begin() + r + 1, cmp);

		BoundingBox currentBox;
		// 求后缀包围盒表面积和
		sufArea[r - l + 1] = 0.f;
		for (int j = r - l; j >= 0; j--)
		{
			currentBox = currentBox.Union(startP[j]->GetBoundingBox());
			sufArea[j] = currentBox.SurfaceArea();
		}


		// 扫描线，动态求出最小cost
		currentBox = startP[0]->GetBoundingBox();
		for (int j = 1; j <= r - l; j++)
		{
			float cost = TRAV_COST + (currentBox.SurfaceArea() * j + sufArea[j] * (r - l + 1 - j)) / totArea * INTERSECT_COST;
			if (cost < minCost)
			{
				splitAxis = i;
				splitPos = l + j - 1;
				minCost = cost;
			}
			if (j != r - l)
			{
				currentBox = currentBox.Union(startP[j]->GetBoundingBox());
			}
		}
	}
	delete[] sufArea;
	return minCost < (r - l + 1) * INTERSECT_COST;
}

void BVH::createLeaf(int& p, int l, int r)
{
	BoundingBox box = _objects[l]->GetBoundingBox();
	// 获取[l, r]的碰撞箱，并且把纯指针放进shapes里
	for (int i = l + 1; i <= r; i++)
	{
		box = box.Union(_objects[i]->GetBoundingBox());
	}
	p = newNode(l, r - l + 1, -1, box);
}
