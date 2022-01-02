#pragma once
#include "AreaLight.h"

namespace crystal
{
    class DiffusedAreaLight : public AreaLight
    {
    public:
        DiffusedAreaLight(const Shape* shape, const Spectrum& emitRadiance, int numSamples);

        Spectrum EvalEmitRadiance(const SurfaceInfo& surface_l, const Vector3f& w) const override;
        Spectrum Sample_Li(const SurfaceInfo& surface_w, const glm::vec2& sample, glm::vec3* endpoint, float* pdf) const override;
        float Pdf_Li(const SurfaceInfo& surface_w, const Vector3f& wi) const override;

        Spectrum Flux() const override;

    private:
        Spectrum _emitRadiance;
        const Shape* _shape;
        float _area;
    };
}