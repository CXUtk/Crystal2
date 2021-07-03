#include "BVH.h"
#include <Core/Geometry.h>
#include <Shapes/Shape.h>
#include <algorithm>

namespace accel {
    enum class SplitMethod {
        EQUAL_COUNTS,
        SAH
    };
    constexpr int MAX_OBJ_IN_NODE = 2;
    constexpr SplitMethod SLILT_METHOD = SplitMethod::SAH;
    constexpr float TRAV_COST = 0.5f;
    constexpr float INTERSECT_COST = 1.0f;

    struct BVHNode {
        BoundingBox bound;
        int shapeStartOffset, count;
        int ch[2];
        int splitAxis;
    };
};

BVH::BVH() : _nodes(nullptr), _tot(0), _root(0) {
}

BVH::~BVH() {
    if (_nodes)
        delete[] _nodes;
}

void BVH::Build(const std::vector<std::shared_ptr<Shape>>& objects) {
    for (auto& obj : objects) {
        _shapes.push_back(obj.get());
    }
    _nodes = new accel::BVHNode[objects.size() * 5];
    _tot = 0;
    _build(_root, 0, objects.size() - 1);
}

bool BVH::Intersect(const Ray& ray, SurfaceInteraction* info, float tMin, float tMax) const {
    if (!RayBoxTest(ray, _nodes[_root].bound, tMin, tMax)) return false;
    return _intersect(_root, ray, info, tMin, tMax);
}

bool BVH::IntersectTest(const Ray& ray, float tMin, float tMax) const {
    if (!RayBoxTest(ray, _nodes[_root].bound, tMin, tMax)) return false;
    return _intersectP(_root, ray, tMin, tMax);
}

void BVH::_build(int& p, int l, int r) {
    if (r - l + 1 <= accel::MAX_OBJ_IN_NODE) {
        const Shape* shapes[accel::MAX_OBJ_IN_NODE];
        shapes[0] = _shapes[l];
        BoundingBox box = _shapes[l]->GetBoundingBox();
        // 获取[l, r]的碰撞箱，并且把纯指针放进shapes里
        for (int i = l + 1; i <= r; i++) {
            shapes[i - l] = _shapes[i];
            box = box.Union(_shapes[i]->GetBoundingBox());
        }
        p = newNode(l, r - l + 1, -1, box);
        return;
    }
    // 获取[l, r]的碰撞箱
    BoundingBox box = _shapes[l]->GetBoundingBox();
    for (int i = l + 1; i <= r; i++) {
        box = box.Union(_shapes[i]->GetBoundingBox());
    }

    int splitAxis, splitPos;
    if constexpr (accel::SLILT_METHOD == accel::SplitMethod::EQUAL_COUNTS) {
        splitAxis = splitByEqualCount(l, r, box, splitPos);
    }
    else {
        splitAxis = splitBySAH(l, r, box, splitPos);
    }

    // 按照排序结果从中间分割物体们
    p = newNode(0, 0, splitAxis, box);
    _build(_nodes[p].ch[0], l, splitPos);
    _build(_nodes[p].ch[1], splitPos + 1, r);
}

int BVH::newNode(int offset, int cnt, int splitAxis, const BoundingBox& box) {
    _tot++;
    _nodes[_tot].shapeStartOffset = offset;
    _nodes[_tot].bound = box;
    _nodes[_tot].count = cnt;
    _nodes[_tot].splitAxis = splitAxis;
    return _tot;
}

bool BVH::_intersect(int p, const Ray& ray, SurfaceInteraction* info, float tMin, float tMax) const {
    int objCnt = _nodes[p].count;
    bool hit = false;
    // 如果是叶子节点就是暴力判定一下
    if (objCnt != 0) {
        SurfaceInteraction isec;
        for (int i = 0; i < objCnt; i++) {
            float t1 = tMin, t2 = std::min(tMax, info->GetDistance());
            const Shape* const* startP = &_shapes[_nodes[p].shapeStartOffset];
            if (!RayBoxTest(ray, startP[i]->GetBoundingBox(), t1, t2)) continue;
            if (!startP[i]->Intersect(ray, &isec)) continue;
            if (isec.GetDistance() < info->GetDistance()) {
                *info = std::move(isec);
            }
            hit = true;
        }
        return hit;
    }

    float t1 = tMin, t2 = std::min(tMax, info->GetDistance());
    if (RayBoxTest(ray, _nodes[_nodes[p].ch[0]].bound, t1, t2)) {
        hit |= _intersect(_nodes[p].ch[0], ray, info, t1, t2);
    }

    t1 = tMin, t2 = std::min(tMax, info->GetDistance());
    if (RayBoxTest(ray, _nodes[_nodes[p].ch[1]].bound, t1, t2)) {
        hit |= _intersect(_nodes[p].ch[1], ray, info, t1, t2);
    }

    return hit;
}

bool BVH::_intersectP(int p, const Ray& ray, float tMin, float tMax) const {
    int objCnt = _nodes[p].count;
    // 如果是叶子节点就是暴力判定一下
    if (objCnt != 0) {
        SurfaceInteraction isec;
        for (int i = 0; i < objCnt; i++) {
            float t1 = tMin, t2 = tMax;
            const Shape* const* startP = &_shapes[_nodes[p].shapeStartOffset];
            if (!RayBoxTest(ray, startP[i]->GetBoundingBox(), t1, t2)) continue;
            if (!startP[i]->Intersect(ray, &isec)) continue;
            return true;
        }
        return false;
    }

    bool hit = false;
    float t1 = tMin, t2 = tMax;
    if (RayBoxTest(ray, _nodes[_nodes[p].ch[0]].bound, t1, t2)) {
        hit |= _intersectP(_nodes[p].ch[0], ray, t1, t2);
    }
    if (hit) return true;
    if (RayBoxTest(ray, _nodes[_nodes[p].ch[1]].bound, tMin, tMax)) {
        hit |= _intersectP(_nodes[p].ch[1], ray, tMin, tMax);
    }
    return hit;
}

int BVH::splitByEqualCount(int l, int r, const BoundingBox& box, int& splitPos) {
    int splitAxis = box.MaxExtent();
    // 按照某种方式分割物体，并且排序
    auto cmp = [splitAxis](const Shape* a, const Shape* b) {
        return a->GetBoundingBox().GetCenter()[splitAxis] < b->GetBoundingBox().GetCenter()[splitAxis];
    };
    splitPos = l + (r - l) / 2;
    std::nth_element(_shapes.begin() + l, _shapes.begin() + splitPos, _shapes.begin() + r + 1, cmp);
    return splitAxis;
}

int BVH::splitBySAH(int l, int r, const BoundingBox& box, int& splitPos) {
    int splitAxis = 0;
    float minCost = std::numeric_limits<float>::infinity();
    const Shape* const* startP = &_shapes[l];
    float totArea = box.SurfaceArea();
    float* sufArea = new float[r - l + 2];
    for (int i = 0; i < 3; i++) {
        // 按照中点坐标排序
        auto cmp = [i](const Shape* a, const Shape* b) {
            return a->GetBoundingBox().GetCenter()[i] < b->GetBoundingBox().GetCenter()[i];
        };
        std::sort(_shapes.begin() + l, _shapes.begin() + r + 1, cmp);

        // 求后缀包围盒表面积和
        sufArea[r - l + 1] = 0.f;
        for (int j = r - l; j >= 0; j--) {
            sufArea[j] = sufArea[j + 1] + startP[j]->GetBoundingBox().SurfaceArea();
        }

        // 扫描线，动态求出最小cost
        float prefArea = startP[0]->GetBoundingBox().SurfaceArea();
        for (int j = 1; j <= r - l; j++) {
            float cost = accel::TRAV_COST + (prefArea * j + sufArea[j] * (r - l + 1 - j)) / totArea * accel::INTERSECT_COST;
            if (cost < minCost) {
                splitAxis = i;
                splitPos = l + j - 1;
                minCost = cost;
            }
            if (j != r - l)
                prefArea += startP[j]->GetBoundingBox().SurfaceArea();
        }
    }
    delete[] sufArea;
    return splitAxis;
}
