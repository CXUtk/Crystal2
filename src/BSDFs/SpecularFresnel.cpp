#include "SpecularFresnel.h"

SpecularFresnel::SpecularFresnel(glm::vec3 R, glm::vec3 T, glm::vec3 N, float etaA, float etaB, const std::shared_ptr<Fresnel>& fresnel)
	: BxDF(BxDFType(BxDFType::BxDF_TRANSMISSION | BxDFType::BxDF_REFLECTION | BxDFType::BxDF_SPECULAR)),
	_R(R), _T(T), _N(N), _etaA(etaA), _etaB(etaB), _fresnel(fresnel)
{

}

SpecularFresnel::~SpecularFresnel()
{}

glm::vec3 SpecularFresnel::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const
{
	return glm::vec3(0);
}


static bool refract(glm::vec3 wo, glm::vec3 N, float etaA, float etaB, glm::vec3& wt)
{
	float eta = etaA / etaB;
	auto cosThetaI = glm::dot(wo, N);
	auto sin2ThetaI = std::max(0.f, 1.f - cosThetaI * cosThetaI);
	auto sin2ThetaT = eta * eta * sin2ThetaI;
	auto cosThetaT = std::sqrt(1.f - sin2ThetaT);
	auto t = glm::dot(wo, N);
	wt = eta * (-wo) + (eta * glm::dot(wo, N) - cosThetaT) * N;
	return true;
}



glm::vec3 SpecularFresnel::SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const
{
	auto fr = _fresnel->Eval(_etaA, _etaB, glm::dot(_N, wOut));
	*pdf = 1.0f;
	if (sample[0] < fr.r)
	{
		*sampledType = BxDFType(BxDFType::BxDF_REFLECTION | BxDFType::BxDF_SPECULAR);
		*wIn = glm::reflect(-wOut, _N);
		return _R;
	}
	else
	{
		*sampledType = BxDFType(BxDFType::BxDF_REFLECTION | BxDFType::BxDF_SPECULAR);
		refract(wOut, _N, _etaA, _etaB, *wIn);
		return _T;
	}
}
