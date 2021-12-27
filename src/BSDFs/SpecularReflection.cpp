#include "SpecularReflection.h"


SpecularReflection::SpecularReflection(glm::vec3 color, float etaA, float etaB, const std::shared_ptr<Fresnel>& fresnel) :
	BxDF(BxDFType(BxDFType::BxDF_SPECULAR | BxDFType::BxDF_REFLECTION)), _albedo(color), _etaA(etaA), _etaB(etaB),
	_fresnel(fresnel)
{}

SpecularReflection::~SpecularReflection()
{}

float SpecularReflection::Pdf(glm::vec3 wOut, glm::vec3 wIn) const
{
	return 0.0f;
}

Spectrum SpecularReflection::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const
{
	return glm::vec3(0);
}

Spectrum SpecularReflection::SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const
{
	*wIn = Vector3f(-wOut.x, wOut.y, -wOut.z);
	*pdf = 1.0f;
	return _albedo * _fresnel->Eval(_etaA, _etaB, wOut.y);
}
