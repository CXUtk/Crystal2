#include "Reflections.h"
#include <glm/gtx/transform.hpp>
#include <Core/Utils.h>

namespace crystal
{
	static float sinTheta(Vector3f w)
	{
		return std::sqrt(std::max(0.f, 1.f - w.y * w.y));
	}

	static Vector2f getCosSinPhi(Vector3f w, float sinTheta)
	{
		if (sinTheta == 0.f) return Vector2f(1.f, 0.f);
		return glm::clamp(Vector2f(w.x / sinTheta, -w.z / sinTheta),
			glm::vec2(-1, -1), glm::vec2(1, 1));
	}

	OrenNayarReflection::OrenNayarReflection(Spectrum color, float sigma)
		: BxDF(static_cast<BxDFType>(BxDFType::BxDF_DIFFUSE | BxDFType::BxDF_REFLECTION)), 
		_albedo(color), _sigma(sigma)
	{
		float sigmaSq = _sigma * _sigma;
		_A = 1.f - sigmaSq / (2 * (sigmaSq + 0.33f));
		_B = (0.45f * sigmaSq) / (sigmaSq + 0.09f);
	}

	OrenNayarReflection::~OrenNayarReflection()
	{}

	Spectrum OrenNayarReflection::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const
	{
		float sinThetaO = sinTheta(wOut);
		float sinThetaI = sinTheta(wIn);
		float sinAlpha, tanBeta;
		if (wOut.y < wIn.y)
		{
			sinAlpha = sinThetaI;
			tanBeta = sinThetaO / wOut.y;
		}
		else
		{
			sinAlpha = sinThetaO;
			tanBeta = sinThetaI / wIn.y;
		}

		float dCos = 0.f;
		if (sinThetaI > EPS && sinThetaO > EPS)
		{
			Vector2f dI = getCosSinPhi(wIn, sinThetaI), dO = getCosSinPhi(wOut, sinThetaO);
			dCos = std::max(0.f, glm::dot(dI, dO));
		}

		return _albedo / glm::pi<float>() * (_A + _B * (dCos * sinAlpha * tanBeta));
	}

	Spectrum OrenNayarReflection::SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const
	{
		*wIn = NextCosineUnitHemiSphere(sample, pdf);
		return DistributionFunction(wOut, *wIn);
	}
}
