#pragma once
#include <glm/glm.hpp>
#include <Crystal2.h>
#include "BSDF.h"


class SpecularTransmission : public BxDF {
public:
    SpecularTransmission(glm::vec3 color, glm::vec3 N, float etaA, float etaB);
    ~SpecularTransmission() override;

    glm::vec3 DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const override;
    glm::vec3 SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf) const override;

private:
    glm::vec3 _albedo, _normal;
    float _etaA, _etaB;
};