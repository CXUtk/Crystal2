﻿#pragma once
#include "Light.h"

namespace crystal
{
    class AreaLight : public Light
    {
    public:
        AreaLight(int numSamples) : Light(LightFlags::Area, numSamples) {}
        virtual Spectrum EvalEmission(const SurfaceInfo& surface, const Vector3f& w) const = 0;
    };
}