#include "SpecularFresnel.h"

SpecularFresnel::SpecularFresnel(glm::vec3 R, glm::vec3 T, float etaA, float etaB, const std::shared_ptr<Fresnel>& fresnel)
	: BxDF(BxDFType(BxDFType::BxDF_TRANSMISSION | BxDFType::BxDF_REFLECTION | BxDFType::BxDF_SPECULAR)),
	_R(R), _T(T), _etaA(etaA), _etaB(etaB), _fresnel(fresnel)
{

}

SpecularFresnel::~SpecularFresnel()
{}

glm::vec3 SpecularFresnel::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const
{
	return glm::vec3(0);
}


static void refract(glm::vec3 wo, float etaA, float etaB, glm::vec3* wt)
{
	float eta = etaA / etaB;
	wo *= -eta;
	auto sin2ThetaT = wo.x * wo.x + wo.z * wo.z;
	wo.y = -std::sqrt(1.f - sin2ThetaT);
	*wt = wo;
}




glm::vec3 SpecularFresnel::SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const
{
	auto fr = _fresnel->Eval(_etaA, _etaB, glm::dot(Vector3f(0, 1.f, 0), wOut));
	*pdf = 1.0f;
	if (sample[0] < fr.r)
	{
		*sampledType = BxDFType(BxDFType::BxDF_REFLECTION | BxDFType::BxDF_SPECULAR);
		*wIn = glm::reflect(-wOut, Vector3f(0, 1.f, 0));
		return _R;
	}
	else
	{
		*sampledType = BxDFType(BxDFType::BxDF_TRANSMISSION | BxDFType::BxDF_SPECULAR);
		refract(wOut, _etaA, _etaB, wIn);
		return _T;
	}
}
