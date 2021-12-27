#include "MicrofacetReflection.h"
#include <Core/Utils.h>



MicrofacetReflection::MicrofacetReflection(glm::vec3 color, float etaA, float etaB, const std::shared_ptr<Fresnel>& fresnel,
	const std::shared_ptr<MicrofacetDistribution>& distribution) 
	: BxDF(BxDFType(BxDFType::BxDF_REFLECTION | BxDFType::BxDF_GLOSSY)),
	_R(color),  _etaA(etaA), _etaB(etaB), _fresnel(fresnel), _microDistribution(distribution)
{

}

MicrofacetReflection::~MicrofacetReflection()
{}

float MicrofacetReflection::Pdf(glm::vec3 wOut, glm::vec3 wIn) const
{
	auto H = glm::normalize(wOut + wIn);
	return _microDistribution->Pdf(wOut, H) / (4 * glm::dot(wOut, H));
}

glm::vec3 MicrofacetReflection::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const
{
	if (wIn.y <= 0 || wOut.y <= 0) return glm::vec3(0);
	auto H = glm::normalize(wOut + wIn);

	auto F = _fresnel->Eval(_etaA, _etaB, std::max(0.f, glm::dot(wIn, H)));
	auto G = _microDistribution->G(wOut, wIn);
	auto D = _microDistribution->D(H);
	auto v = _R * D * G * F / (4 * std::max(0.f, wIn.y) * std::max(0.f, wOut.y));
	if (glm::isnan(v) != glm::bvec3(false))
	{
		printf("NAN: %lf, %lf, %lf, %lf, %lf\n", wOut.x, wOut.y, wOut.z, G, wIn.x);
	}
	NAN_DETECT_V(v, "MicrofacetReflection::DistributionFunction");
	INF_DETECT_V(v, "MicrofacetReflection::DistributionFunction");
	return v;
}



glm::vec3 MicrofacetReflection::SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const
{
	auto wh = _microDistribution->Sample_wh(wOut, sample);
	if (glm::dot(wOut, wh) <= 0) return glm::vec3(0.f);
	auto L = glm::reflect(-wOut, wh);
	*pdf = _microDistribution->Pdf(L, wh) / (4 * glm::dot(wOut, wh));
	*wIn = L;
	if (glm::isnan(*pdf) || glm::isnan(*wIn) != glm::bvec3(false))
	{
		assert(false);
	}
	fixVector(*wIn);
	return DistributionFunction(wOut, *wIn);
}
