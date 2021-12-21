#include "MicrofacetReflection.h"
#include <Core/Utils.h>



MicrofacetReflection::MicrofacetReflection(glm::vec3 color, float etaA, float etaB, const std::shared_ptr<Fresnel>& fresnel,
	const std::shared_ptr<MicrofacetDistribution>& distribution) : BxDF(BxDFType(BxDFType::BxDF_REFLECTION | BxDFType::BxDF_GLOSSY)),
	_R(color),  _etaA(etaA), _etaB(etaB), _fresnel(fresnel), _microDistribution(distribution)
{

}

MicrofacetReflection::~MicrofacetReflection()
{}

glm::vec3 MicrofacetReflection::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const
{
	if (wIn.y <= 0 || wOut.y <= 0) return glm::vec3(0);
	auto H = glm::normalize(wOut + wIn);

	auto F = _fresnel->Eval(_etaA, _etaB, std::max(0.f, glm::dot(wOut, H)));
	auto G = _microDistribution->G(wOut, wIn);
	auto D = _microDistribution->D(H);
	return _R * (D * G * F / (4 * std::max(0.f, wIn.y) * std::max(0.f, wOut.y)));
}



glm::vec3 MicrofacetReflection::SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const
{
	*pdf = 0.f;
	return Spectrum(0.f);
	auto H = _microDistribution->Sample_wh(wOut, sample);
	if (glm::dot(wOut, H) <= 0) return glm::vec3(0.f);
	auto L = glm::reflect(-wOut, H);
	*pdf = _microDistribution->Pdf(L, H);
	*wIn = L;
	if (glm::isnan(*pdf))
	{
		assert(false);
	}
	return DistributionFunction(wOut, *wIn);
}
