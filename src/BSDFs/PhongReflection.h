#pragma once
#include "BSDF.h"

#include <Crystal2.h>
#include <BSDFs/Models/Fresnel.h>

class PhongReflection : public BxDF
{
public:
    PhongReflection(glm::vec3 color, const glm::mat3& TNB, int k);
    ~PhongReflection() override;

    glm::vec3 DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const override;
    glm::vec3 SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const override;

private:
    glm::vec3 _albedo;
    glm::mat3 _TNB;
    int _k;
};
