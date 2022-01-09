#pragma once
#include "Accelerator.h"

class Brute : public Accelerator {
public:
    Brute();
    ~Brute() override;
    void Build(const std::vector<const crystal::IIntersectable*>& objects) override;
    bool Intersect(const Ray& ray, SurfaceInteraction* info, 
        float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;
    bool IntersectTest(const Ray& ray, const crystal::IIntersectable* ignoreShape, 
        float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;

private:
    std::vector<const crystal::IIntersectable*> _objects;
};
