#include "FresnelBlend.h"
#include <glm/gtx/transform.hpp>
#include <Core/Utils.h>

FresnelBlend::FresnelBlend(const Spectrum& rD, const Spectrum& rS, 
	const std::shared_ptr<MicrofacetDistribution>& distribution) :
	BxDF(static_cast<BxDFType>(BxDFType::BxDF_REFLECTION | BxDFType::BxDF_GLOSSY)), _rD(rD), _rS(rS),
	_microDistribution(distribution)
{
}

FresnelBlend::~FresnelBlend()
{}

float FresnelBlend::Pdf(glm::vec3 wOut, glm::vec3 wIn) const
{
	auto H = glm::normalize(wOut + wIn);
	return 0.5f * (std::max(0.f, wIn.y) / glm::pi<float>() 
		+ _microDistribution->Pdf(wOut, H) / (4 * glm::dot(wOut, H)));
}

Spectrum Schlick(Spectrum R0, float cosThetaI)
{
	return R0 + (Spectrum(1.f) - R0) * std::pow(1.f - cosThetaI, 5.f);
}

Spectrum FresnelBlend::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const
{
	float cosThetaI = std::max(0.f, wIn.y);
	float cosThetaO = std::max(0.f, wOut.y);
	Spectrum diffuse = (28.f / (23.f * glm::pi<float>())) 
		* _rD
		* (Spectrum(1.f) - _rS)
		* (1.f - (float)std::pow(1.f - 0.5f * cosThetaO, 5))
		* (1.f - (float)std::pow(1.f - 0.5f * cosThetaI, 5));

	auto H = glm::normalize(wOut + wIn);
	Spectrum specular = _microDistribution->D(H) /
		(4 * glm::dot(wIn, H) *
		 std::max(cosThetaI, cosThetaO)) *
		Schlick(_rS, glm::dot(wIn, H));
	return diffuse + specular;
}

Spectrum FresnelBlend::SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const
{
	float u = sample.x;
	if (u < 0.5)
	{
		sample.x = u * 2.f;
		*wIn = NextCosineUnitHemiSphere(sample, pdf);
	}
	else
	{
		sample.x = (u - 0.5f) * 2.f;
		auto wh = _microDistribution->Sample_wh(wOut, sample);
		auto L = glm::reflect(-wOut, wh);
		if (L.y * wOut.y <= 0) return Spectrum(0.f);
		*wIn = L;
		if (glm::isnan(*pdf) || glm::isnan(*wIn) != glm::bvec3(false))
		{
			assert(false);
		}
		fixVector(*wIn);
	}
	*pdf = Pdf(wOut, *wIn);
	return DistributionFunction(wOut, *wIn);
}
