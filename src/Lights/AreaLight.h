#pragma once
#include "Light.h"
#include <Core/Entities.h>
#include <Shapes/Shape.h>

namespace crystal
{
    class AreaLight : public Light
    {
    public:
        AreaLight(int numSamples, const Shape* attachedShape = nullptr) 
            : Light(LightFlags::Area, numSamples), _attached(attachedShape) {}
        virtual Spectrum EvalEmitRadiance(const SurfaceInfo& surface, const Vector3f& w) const = 0;

        const IIntersectable* GetAttachedObject() const override { return _attached; }
    private:
        const Shape* _attached;
    };
}