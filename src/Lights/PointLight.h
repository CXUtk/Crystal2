#pragma once
#include "Light.h"

namespace crystal
{
    class PointLight : public Light
    {
    public:
        PointLight(glm::vec3 pos, glm::vec3 flux);
        ~PointLight() override;

        Spectrum Sample_Li(const SurfaceInfo& surface, const glm::vec2& sample,
            Point3f* endpoint, float* pdf) const override;
        float Pdf_Li(const SurfaceInfo& surface, const Vector3f& wi) const override;
        Spectrum Flux() const override;

    private:
        Point3f _pos;
        Spectrum _flux;
    };
}