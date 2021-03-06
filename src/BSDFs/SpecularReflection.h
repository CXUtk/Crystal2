#pragma once
#include <glm/glm.hpp>
#include <Crystal2.h>
#include "BSDF.h"
#include <BSDFs/Models/Fresnel.h>

class SpecularReflection : public BxDF
{
public:
    SpecularReflection(glm::vec3 color, const std::shared_ptr<Fresnel>& fresnel, float etaA, float etaB);
    ~SpecularReflection() override;

    float Pdf(glm::vec3 wOut, glm::vec3 wIn) const override;
    Spectrum DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const override;
    Spectrum SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const override;

private:
    Spectrum _albedo;
    float _etaA, _etaB;
    std::shared_ptr<Fresnel> _fresnel;
};
