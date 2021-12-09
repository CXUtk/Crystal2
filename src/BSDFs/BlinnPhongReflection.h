#pragma once
#include "BSDF.h"

#include <Crystal2.h>
#include <BSDFs/Models/Fresnel.h>

namespace crystal
{
    class BlinnPhongReflection : public BxDF
    {
    public:
        BlinnPhongReflection(glm::vec3 color, const glm::mat3& TNB, int k);
        ~BlinnPhongReflection() override;

        glm::vec3 DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const override;
        glm::vec3 SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const override;

    private:
        glm::vec3 _albedo;
        glm::mat3 _TNB;
        int _k;
    };
}