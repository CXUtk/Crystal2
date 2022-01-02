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
    virtual SurfaceInfo SampleSurface(const Vector2f& sample) const = 0;
    // 采样目标点为半球的立体角
    virtual SurfaceInfo SampleSurfaceLight(const Vector2f& sample,
        const SurfaceInteraction& ref) const = 0;
    virtual float Pdf(const SurfaceInfo& surface_w) const { return 1.f / SurfaceArea(); }
    virtual float PdfLight(const SurfaceInfo& surface_w, const Vector3f& wi) const;

private:
};