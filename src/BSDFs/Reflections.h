#pragma once
#include <glm/glm.hpp>
#include <Crystal2.h>
#include "BSDF.h"

namespace crystal
{
    class OrenNayarReflection : public BxDF
    {
    public:
        OrenNayarReflection(Spectrum color, float sigma);
        ~OrenNayarReflection() override;

        Spectrum DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const override;
        Spectrum SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const override;

    private:
        Spectrum _albedo;
        float _sigma, _A, _B;
    };
}