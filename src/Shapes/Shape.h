#pragma once
#include <Crystal2.h>
#include <Core/Geometry.h>
#include <Core/SurfaceInteraction.h>
#include <memory>

class Shape {
public:
    Shape(Prototype* prototype) : _prototype(prototype) {}
    virtual ~Shape() = 0 {}
    virtual BoundingBox GetBoundingBox() const = 0;
    virtual bool Intersect(const Ray& ray, SurfaceInteraction* info) const = 0;
    virtual bool IntersectTest(const Ray& ray, float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const = 0;

    const Prototype* GetPrototype() const { return _prototype; }

private:
    Prototype* _prototype;
};