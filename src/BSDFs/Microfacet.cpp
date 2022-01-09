#include "Microfacet.h"
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

	auto F = _fresnel->Eval(_etaA, _etaB, std::max(0.f, glm::dot(wOut, H)));
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
	*wIn = glm::reflect(-wOut, wh);
	*pdf = Pdf(wOut, *wIn);// _microDistribution->Pdf(wOut, wh) / (4 * glm::dot(*wIn, wh));
	if (glm::isnan(*pdf) || glm::isnan(*wIn) != glm::bvec3(false))
	{
		assert(false);
	}
	fixVector(*wIn);
	return DistributionFunction(wOut, *wIn);
}

MicrofacetTransmission::MicrofacetTransmission(glm::vec3 color, float etaA, float etaB, 
	const std::shared_ptr<Fresnel>& fresnel, 
	const std::shared_ptr<MicrofacetDistribution>& distribution)
	: BxDF(BxDFType(BxDFType::BxDF_TRANSMISSION | BxDFType::BxDF_GLOSSY)),
	_R(color), _etaA(etaA), _etaB(etaB), _fresnel(fresnel), _microDistribution(distribution)
{}

MicrofacetTransmission::~MicrofacetTransmission()
{}

float MicrofacetTransmission::Pdf(glm::vec3 wOut, glm::vec3 wIn) const
{
	if (wOut.y * wIn.y > 0) return 0.f;
	float eta = _etaB / _etaA;
	auto H = glm::normalize(wOut + wIn * eta);
	auto absDot = std::abs(glm::dot(H, wIn));
	float dwh_dwi = eta * eta
		* absDot
		/ square(glm::dot(wOut, H) + eta * glm::dot(wIn, H));
	return _microDistribution->Pdf(wOut, H) * dwh_dwi;
}

Spectrum MicrofacetTransmission::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const
{
	if (wIn.y * wOut.y > 0) return glm::vec3(0);
	float eta = _etaB / _etaA;
	auto H = glm::normalize(wOut + wIn * eta);
	
	if (H.y < 0) H = -H;

	float bot = square(glm::dot(wOut, H) + eta * glm::dot(wIn, H));
	auto F = Spectrum(1.f) - _fresnel->Eval(_etaA, _etaB, std::max(0.f, glm::dot(wOut, H)));
	auto G = _microDistribution->G(wOut, wIn);
	auto D = _microDistribution->D(H);
	auto v = _R * D * G * F 
		* std::abs(glm::dot(H, wOut)) 
		* std::abs(glm::dot(H, wIn))
		/ (bot * std::max(0.f, -wIn.y) * std::max(0.f, wOut.y));
	if (glm::isnan(v) != glm::bvec3(false))
	{
		printf("NAN: %lf, %lf, %lf, %lf, %lf\n", wOut.x, wOut.y, wOut.z, G, wIn.x);
	}
	NAN_DETECT_V(v, "MicrofacetTransmission::DistributionFunction");
	INF_DETECT_V(v, "MicrofacetTransmission::DistributionFunction");
	return v;
}

Spectrum MicrofacetTransmission::SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const
{
	auto wh = _microDistribution->Sample_wh(wOut, sample);
	if (glm::dot(wOut, wh) <= 0) return Spectrum(0.f);
	float eta = _etaA / _etaB;
	if (refract(wOut, wh, eta, wIn))
	{
		if (glm::isnan(*wIn) != glm::bvec3(false))
		{
			printf("NAN: %lf, %lf, %lf\n", wOut.x, wOut.y, wOut.z);
		}
		*pdf = Pdf(wOut, *wIn);
		fixVector(*wIn);
		return DistributionFunction(wOut, *wIn);
	}
	else
	{
		return Spectrum(0.f);
	}

}
