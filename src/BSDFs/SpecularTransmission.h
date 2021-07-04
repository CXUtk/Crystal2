#pragma once
#include <glm/glm.hpp>
#include <Crystal2.h>
#include <BSDFs/Models/Fresnel.h>
#include "BSDF.h"



class SpecularTransmission : public BxDF {
public:
    SpecularTransmission(glm::vec3 color, glm::vec3 N, const std::shared_ptr<Fresnel>& fresnel, float etaA, float etaB);
    ~SpecularTransmission() override;

    glm::vec3 DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const override;
    glm::vec3 SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const override;

private:
    glm::vec3 _albedo, _normal;
    std::shared_ptr<Fresnel> _fresnel;
    float _etaA, _etaB;
};
