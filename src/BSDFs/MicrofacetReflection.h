#pragma once
#include "BSDF.h"
#include <glm/glm.hpp>
#include <Crystal2.h>
#include <BSDFs/Models/Fresnel.h>
#include <BSDFs/Models/MicrofacetDistribution.h>

class MicrofacetReflection : public BxDF {
public:
    MicrofacetReflection(glm::vec3 color, const glm::mat3& TNB, const std::shared_ptr<Fresnel>& fresnel, 
        const std::shared_ptr<MicrofacetDistribution>& distribution);
    ~MicrofacetReflection() override;

    glm::vec3 DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const override;
    glm::vec3 SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const override;

private:
    glm::vec3 _R;
    glm::mat3 _TNB;
    std::shared_ptr<Fresnel> _fresnel;
    std::shared_ptr<MicrofacetDistribution> _microDistribution;
};
