#pragma once
#include <glm/glm.hpp>
#include <Crystal2.h>
#include "BSDF.h"

class Lambertain : public BxDF {
public:
    Lambertain(glm::vec3 color);
    ~Lambertain() override;

    Spectrum DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const override;
    float Pdf(glm::vec3 wOut, glm::vec3 wIn) const override;
    Spectrum SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const override;

private:
    Spectrum _albedo;
};