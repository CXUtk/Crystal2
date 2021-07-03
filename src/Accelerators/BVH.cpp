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
    constexpr SplitMethod SLILT_METHOD = SplitMethod::EQUAL_COUNTS;

    struct BVHNode {
        const Shape* shapes[MAX_OBJ_IN_NODE];
        int count, ch[2];
        BoundingBox bound;
    };
};

BVH::BVH() : _nodes(nullptr), _tot(0), _root(0) {
}

BVH::~BVH() {
    if (_nodes)
        delete[] _nodes;
}

void BVH::Build(const std::vector<std::shared_ptr<Shape>>& objects) {
    std::vector<const Shape*> objs;
    for (auto& obj : objects) {
        objs.push_back(obj.get());
    }
    _nodes = new accel::BVHNode[objs.size() * 5];
    _tot = 0;
    _build(_root, 0, objs.size() - 1, objs);
}

bool BVH::Intersect(const Ray& ray, SurfaceInteraction* info, float tMin, float tMax) const {
    if (!RayBoxTest(ray, _nodes[_root].bound, tMin, tMax)) return false;
    return _intersect(_root, ray, info, tMin, tMax);
}

bool BVH::IntersectTest(const Ray& ray, float tMin, float tMax) const {
    if (!RayBoxTest(ray, _nodes[_root].bound, tMin, tMax)) return false;
    return _intersectP(_root, ray, tMin, tMax);
}

void BVH::_build(int& p, int l, int r, std::vector<const Shape*>& objs) {
    if (r - l + 1 <= accel::MAX_OBJ_IN_NODE) {
        const Shape* shapes[accel::MAX_OBJ_IN_NODE];
        shapes[0] = objs[l];
        BoundingBox box = objs[l]->GetBoundingBox();
        // 获取[l, r]的碰撞箱，并且把纯指针放进shapes里
        for (int i = l + 1; i <= r; i++) {
            shapes[i - l] = objs[i];
            box = box.Union(objs[i]->GetBoundingBox());
        }
        p = newNode(shapes, r - l + 1, box);
        return;
    }
    // 获取[l, r]的碰撞箱
    BoundingBox box = objs[l]->GetBoundingBox();
    for (int i = l + 1; i <= r; i++) {
        box = box.Union(objs[i]->GetBoundingBox());
    }
    int maxExt = box.MaxExtent();

    // 按照某种方式分割物体，并且排序
    auto cmp = [maxExt](const Shape* a, const Shape* b) {
        return a->GetBoundingBox().GetCenter()[maxExt] < b->GetBoundingBox().GetCenter()[maxExt];
    };
    int mid = l + (r - l) / 2;
    std::nth_element(objs.begin() + l, objs.begin() + mid, objs.begin() + r + 1, cmp);

    // 按照排序结果从中间分割物体们
    p = newNode(nullptr, 0, box);
    _build(_nodes[p].ch[0], l, mid, objs);
    _build(_nodes[p].ch[1], mid + 1, r, objs);
}

int BVH::newNode(const Shape* shape[], int cnt, BoundingBox box) {
    _tot++;
    if (shape) {
        memcpy(_nodes[_tot].shapes, shape, sizeof(const Shape*) * cnt);
    }
    _nodes[_tot].bound = box;
    _nodes[_tot].count = cnt;
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
            if (!RayBoxTest(ray, _nodes[p].shapes[i]->GetBoundingBox(), t1, t2)) continue;
            if (!_nodes[p].shapes[i]->Intersect(ray, &isec)) continue;
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
            if (!RayBoxTest(ray, _nodes[p].shapes[i]->GetBoundingBox(), t1, t2)) continue;
            if (!_nodes[p].shapes[i]->Intersect(ray, &isec)) continue;
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
