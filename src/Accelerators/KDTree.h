#pragma once
#include "Accelerator.h"

struct KDTreeNode;

class KDTree : public Accelerator
{
public:
    KDTree();
    ~KDTree();
    void Build(const std::vector<const crystal::IIntersectable*>& objects) override;
    bool Intersect(const Ray& ray, SurfaceInteraction* info,
        float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;
    bool IntersectTest(const Ray& ray, const crystal::IIntersectable* ignoreShape,
        float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;

private:

    int _tot, _root;
    KDTreeNode* _nodes;
    std::vector<const crystal::IIntersectable*> _objects;
    BoundingBox _masterBox;

    int newNode(const std::vector<const crystal::IIntersectable*>& objs, 
        const BoundingBox& box, int split, float splitPos);
    void push_up(int p);
    void _build(int& p, const BoundingBox& outerBox, 
        std::vector<const crystal::IIntersectable*>& objs, int depth);
    bool ray_test(int p, const Ray& ray, SurfaceInteraction* info, float tMin, float tMax) const;
    bool ray_test_p(int p, const Ray& ray, const crystal::IIntersectable* ignoreShape, 
        float tMin, float tMax) const;
};

