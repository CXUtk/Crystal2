#pragma once
#include <Core/Geometry.h>
#include <Core/SurfaceInteraction.h>

class Shape {
public:
    virtual ~Shape() = 0 {}
    virtual BoundingBox GetBoundingBox() const = 0;
    virtual bool Intersect(const Ray& ray, SurfaceInteraction* info) const = 0;
    virtual bool IntersectTest(const Ray& ray, float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const = 0;
};