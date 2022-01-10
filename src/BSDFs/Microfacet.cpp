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
	if (wOut.y * wIn.y < 0) return Spectrum(0.f);
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
	if (wIn.y * wOut.y > 0) return Spectrum(0);
	if (wOut.y == 0.f) return Spectrum(0.f);
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
		printf("NAN: Wout: [%lf, %lf, %lf], G: %lf, wIn: [%lf %lf %lf], D: %lf\n", wOut.x, wOut.y, wOut.z, G, wIn.x, wIn.y, wIn.z, D);
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

MicrofacetBlend::MicrofacetBlend(glm::vec3 color, float etaA, float etaB, 
	const std::shared_ptr<Fresnel>& fresnel, const std::shared_ptr<MicrofacetDistribution>& distribution)
	: BxDF(BxDFType(BxDFType::BxDF_TRANSMISSION | BxDFType::BxDF_REFLECTION | BxDFType::BxDF_GLOSSY)),
	_R(color), _etaA(etaA), _etaB(etaB), _fresnel(fresnel), _microDistribution(distribution)
{
	_reflection = std::make_shared<MicrofacetReflection>(_R, etaA, etaB, fresnel, _microDistribution);
	_transmission = std::make_shared<MicrofacetTransmission>(_R, etaA, etaB, fresnel, _microDistribution);
}

MicrofacetBlend::~MicrofacetBlend()
{}

float MicrofacetBlend::Pdf(glm::vec3 wOut, glm::vec3 wIn) const
{
	if (wOut.y * wIn.y < 0)
	{
		return _transmission->Pdf(wOut, wIn);
	}
	else
	{
		return _reflection->Pdf(wOut, wIn);
	}
}

Spectrum MicrofacetBlend::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const
{
	if (wOut.y * wIn.y < 0)
	{
		return _transmission->DistributionFunction(wOut, wIn);
	}
	else
	{
		return _reflection->DistributionFunction(wOut, wIn);
	}
}


Spectrum MicrofacetBlend::SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const
{
	auto wh = _microDistribution->Sample_wh(wOut, sample);
	if (glm::dot(wOut, wh) <= 0) return Spectrum(0.f);
	float fr = _fresnel->Eval(_etaA, _etaB, glm::dot(wOut, wh)).r;
	if (sample.x < fr)
	{
		*sampledType = BxDFType(BxDFType::BxDF_REFLECTION | BxDFType::BxDF_GLOSSY);

		*wIn = glm::reflect(-wOut, wh);
		*pdf = Pdf(wOut, *wIn) * fr;
		fixVector(*wIn);
		return _reflection->DistributionFunction(wOut, *wIn);
	}
	else
	{
		float eta = _etaA / _etaB;
		if (refract(wOut, wh, eta, wIn))
		{
			*sampledType = BxDFType(BxDFType::BxDF_TRANSMISSION | BxDFType::BxDF_GLOSSY);

			*pdf = Pdf(wOut, *wIn) * (1- fr);
			fixVector(*wIn);
			return _transmission->DistributionFunction(wOut, *wIn);
		}
		else
		{
			*sampledType = BxDFType(BxDFType::BxDF_REFLECTION | BxDFType::BxDF_GLOSSY);

			*wIn = glm::reflect(-wOut, wh);
			*pdf = Pdf(wOut, *wIn) *(1-fr);
			fixVector(*wIn);
			return _reflection->DistributionFunction(wOut, *wIn);
		}
	}
}
