#pragma once
#include <Crystal2.h>
#include <Core/Geometry.h>
#include <Core/Entities.h>
#include <Core/SurfaceInteraction.h>
#include <memory>

class Shape : public crystal::IIntersectable {
public:
    virtual ~Shape() = 0 {}
    virtual float SurfaceArea() const = 0;
    virtual SurfaceInfo SampleSurface(const Vector2f& sample, float* pdf) const = 0;

private:
};