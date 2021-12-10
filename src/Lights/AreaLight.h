#pragma once
#include "Light.h"

namespace crystal
{
    class AreaLight : public Light
    {
    public:
        AreaLight(int numSamples) : Light(LightFlags::Area, numSamples) {}
        virtual glm::vec3 EvalEmission(const SurfaceInteraction& isec) const = 0;
    };
}