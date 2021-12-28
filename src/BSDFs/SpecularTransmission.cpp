#include "SpecularTransmission.h"
#include <Core/Utils.h>

SpecularTransmission::SpecularTransmission(glm::vec3 color, const std::shared_ptr<Fresnel>& fresnel,
	float etaA, float etaB)
	: BxDF(BxDFType(BxDFType::BxDF_SPECULAR | BxDFType::BxDF_TRANSMISSION)),
	_albedo(color), _fresnel(fresnel), _etaA(etaA), _etaB(etaB)
{

}

SpecularTransmission::~SpecularTransmission()
{}

float SpecularTransmission::Pdf(glm::vec3 wOut, glm::vec3 wIn) const
{
	return 0.0f;
}

glm::vec3 SpecularTransmission::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const
{
	return glm::vec3(0);
}


Spectrum SpecularTransmission::SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const
{
	*pdf = 1.0f;
	if (!refract(wOut, _etaA, _etaB, wIn)) return Spectrum(0.f);
	return _albedo * (1.f - _fresnel->Eval(_etaA, _etaB, wOut.y));
}
