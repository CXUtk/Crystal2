#pragma once
#include <glm/glm.hpp>
#include <Crystal2.h>
#include "BSDF.h"

class SpecularReflection : public BxDF {
public:
    SpecularReflection(glm::vec3 color, glm::vec3 N);
    ~SpecularReflection() override;

    glm::vec3 DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const override;
    glm::vec3 SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf) const override;

private:
    glm::vec3 _albedo, _normal;
};