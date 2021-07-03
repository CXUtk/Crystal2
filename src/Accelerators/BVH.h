﻿#pragma once
#include "Accelerator.h"

namespace accel {
    struct BVHNode;
};

class BVH : public Accelerator {
public:
    BVH();
    ~BVH() override;
    void Build(const std::vector<std::shared_ptr<Shape>>& objects) override;
    bool Intersect(const Ray& ray, SurfaceInteraction* info, float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;
    bool IntersectTest(const Ray& ray, float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;

private:
    accel::BVHNode* _nodes;
    int _tot, _root;
    std::vector<const Shape*> _shapes;

    void _build(int& p, int l, int r);
    int newNode(int startOffset, int cnt, int splitAxis, const BoundingBox& box);

    bool _intersect(int p, const Ray& ray, SurfaceInteraction* info, float tMin, float tMax) const;
    bool _intersectP(int p, const Ray& ray, float tMin, float tMax) const;


    int splitByEqualCount(int l, int r, const BoundingBox& box, int& splitPos);
    int splitBySAH(int l, int r, const BoundingBox& box, int& splitPos);
};
