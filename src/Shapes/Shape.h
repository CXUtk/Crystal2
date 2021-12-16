#pragma once
#include <Crystal2.h>
#include <Core/Geometry.h>
#include <Core/SurfaceInteraction.h>
#include <memory>

class Shape {
public:
    virtual ~Shape() = 0 {}
    virtual BoundingBox GetBoundingBox() const = 0;
    virtual bool Intersect(const Ray& ray, SurfaceInteraction* isec) const = 0;
    virtual bool IntersectTest(const Ray& ray, float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const = 0;
    virtual float SurfaceArea() const = 0;
    virtual SurfaceInfo SampleSurface(const Vector2f& sample, float* pdf) const = 0;

private:
};