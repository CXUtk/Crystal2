#pragma once
#include "AreaLight.h"

namespace crystal
{
    class DiffusedAreaLight : public AreaLight
    {
    public:
        DiffusedAreaLight(const Shape* shape, Spectrum flux, int numSamples);

        Spectrum EvalEmission(const SurfaceInteraction& isec, const Vector3f& w) const override;
        Spectrum Sample_Li(const SurfaceInteraction& hit, const glm::vec2& sample, glm::vec3* endpoint, float* pdf) const override;
        Spectrum Flux() const override { return _flux; }

    private:
        Spectrum _flux;
        const Shape* _shape;
        float _area;

        float normalizeFactor() const;
    };
}