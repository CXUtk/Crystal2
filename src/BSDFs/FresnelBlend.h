#pragma once
#include "BSDF.h"
#include <glm/glm.hpp>
#include <Crystal2.h>
#include <BSDFs/Models/Fresnel.h>
#include <BSDFs/Models/MicrofacetDistribution.h>

class FresnelBlend : public BxDF
{
public:
    FresnelBlend(const Spectrum& rD, const Spectrum& rS, 
        const std::shared_ptr<MicrofacetDistribution>& distribution);
    ~FresnelBlend() override;

    float Pdf(glm::vec3 wOut, glm::vec3 wIn) const override;
    Spectrum DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const override;
    Spectrum SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const override;

private:
    Spectrum _rD, _rS;
    std::shared_ptr<MicrofacetDistribution> _microDistribution;
};
