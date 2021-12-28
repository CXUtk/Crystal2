#include "SpecularFresnel.h"
#include <Core/Utils.h>

SpecularFresnel::SpecularFresnel(glm::vec3 R, glm::vec3 T, const std::shared_ptr<Fresnel>& fresnel, 
	float etaA, float etaB)
	: BxDF(BxDFType(BxDFType::BxDF_TRANSMISSION | BxDFType::BxDF_REFLECTION | BxDFType::BxDF_SPECULAR)),
	_R(R), _T(T), _fresnel(fresnel), _etaA(etaA), _etaB(etaB)
{

}

SpecularFresnel::~SpecularFresnel()
{}

float SpecularFresnel::Pdf(glm::vec3 wOut, glm::vec3 wIn) const
{
	return 0.0f;
}

glm::vec3 SpecularFresnel::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const
{
	return glm::vec3(0);
}




glm::vec3 SpecularFresnel::SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const
{
	auto fr = _fresnel->Eval(_etaA, _etaB, wOut.y);

	if (sample[0] < fr.r)
	{
		*sampledType = BxDFType(BxDFType::BxDF_REFLECTION | BxDFType::BxDF_SPECULAR);
		*wIn = Vector3f(-wOut.x, wOut.y, -wOut.z);
		*pdf = fr.r;
		return fr.r * _R;
	}
	else
	{
		*sampledType = BxDFType(BxDFType::BxDF_TRANSMISSION | BxDFType::BxDF_SPECULAR);
		refract(wOut, Normal3f(0, 1, 0), _etaA / _etaB, wIn);
		*pdf = 1.f - fr.r;
		return (1.f - fr.r) * _T;
	}
}
