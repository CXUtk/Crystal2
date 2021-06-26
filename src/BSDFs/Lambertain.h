#pragma once
#include <glm/glm.hpp>
#include <Crystal2.h>
#include "BSDF.h"

class Lambertain : public BxDF {
public:
    Lambertain(glm::vec3 color, glm::mat3 TNB);
    ~Lambertain() override;

    glm::vec3 DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const override;
    glm::vec3 SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf) const override;

private:
    glm::vec3 _albedo;
    glm::mat3 _TNB;
};