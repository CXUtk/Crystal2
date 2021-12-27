#pragma once
#include <glm/glm.hpp>
#include <Crystal2.h>
#include <BSDFs/Models/Fresnel.h>
#include "BSDF.h"



class SpecularTransmission : public BxDF
{
public:
	SpecularTransmission(glm::vec3 color, const std::shared_ptr<Fresnel>& fresnel, float etaA, float etaB);
	~SpecularTransmission() override;

    float Pdf(glm::vec3 wOut, glm::vec3 wIn) const override;
    Spectrum DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const override;
    Spectrum SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const override;

private:
    Spectrum _albedo;
    std::shared_ptr<Fresnel> _fresnel;
    float _etaA, _etaB;
};
