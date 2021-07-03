#pragma once
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

    void _build(int& p, int l, int r, std::vector<const Shape*>& objs);
    int newNode(const Shape* shape[], int cnt, BoundingBox box);

    bool _intersect(int p, const Ray& ray, SurfaceInteraction* info, float tMin, float tMax) const;
    bool _intersectP(int p, const Ray& ray, float tMin, float tMax) const;
};
