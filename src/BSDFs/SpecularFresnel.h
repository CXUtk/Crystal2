#pragma once
#include <glm/glm.hpp>
#include <Crystal2.h>
#include "BSDF.h"
#include <BSDFs/Models/Fresnel.h>

class SpecularFresnel : public BxDF {
public:
    SpecularFresnel(glm::vec3 R, glm::vec3 T, const std::shared_ptr<Fresnel>& fresnel,float etaA, float etaB);
    ~SpecularFresnel() override;

    float Pdf(glm::vec3 wOut, glm::vec3 wIn) const override;
    glm::vec3 DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const override;
    glm::vec3 SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const override;

private:
    glm::vec3 _R, _T;
    float _etaA, _etaB;
    std::shared_ptr<Fresnel> _fresnel;
};
