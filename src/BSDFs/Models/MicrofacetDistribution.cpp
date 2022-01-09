#include "MicrofacetDistribution.h"
#include <glm/gtx/transform.hpp>
#include <algorithm>
#include <Core/Utils.h>

static Vector2f getCosSinPhi(Vector3f w, float sinTheta)
{
	if (sinTheta == 0.f) return Vector2f(1.f, 0.f);
	return glm::clamp(Vector2f(w.x / sinTheta, -w.z / sinTheta),
		glm::vec2(-1, -1), glm::vec2(1, 1));
}



GGXDistribution::GGXDistribution(Vector2f roughness) : _alpha(roughness)
{}

GGXDistribution::~GGXDistribution()
{}

float GGXDistribution::D(const Vector3f & wh) const
{
	float cos2Theta = wh.y * wh.y;
	assert(1.f - cos2Theta >= 0.f);
	float sin2Theta = std::max(0.f, 1.f - cos2Theta);
	auto phi = getCosSinPhi(wh, std::sqrt(sin2Theta));
	float A = square(phi.x) / square(_alpha.x) + square(phi.y) / square(_alpha.y);
	return 1.f / (glm::pi<float>() * _alpha.x * _alpha.y * square(A - cos2Theta * (A - 1)));
	//return 1.f / (glm::pi<float>() * _alpha.x * _alpha.y * square(1 + sin2Theta * (A - 1)));
	//return  _alpha.x * _alpha.y / (glm::pi<float>() * square(cos2Theta * (_alpha.x * _alpha.y - 1) + 1));
	//return  1.f / (glm::pi<float>() * _alpha.x * _alpha.y * cos2Theta * cos2Theta 
	//	* square(1 + sin2Theta / cos2Theta * A));
}

float GGXDistribution::G(const Vector3f& wo, const Vector3f& wi) const
{
	return 1.f / (1.f + lambda(wo) + lambda(wi));
}

Vector3f GGXDistribution::Sample_wh(const Vector3f& wo, glm::vec2 sample) const
{
	float cosTheta, phi;
	if (_alpha.x == _alpha.y)
	{
		cosTheta = std::sqrt((1 - sample.x) / (sample.x * (_alpha.x * _alpha.x - 1) + 1));
		//cosTheta = std::sqrt(_alpha.x * _alpha.x / (sample.x * (_alpha.x * _alpha.x - 1) + 1));
		NAN_DETECT_V(cosTheta, "GGXDistribution::Sample_wh");

		phi = glm::two_pi<float>() * sample.y;
	}
	else
	{
		phi = std::atan(_alpha.y / _alpha.x *
					  std::tan(glm::two_pi<float>() * sample.y + glm::half_pi<float>()));
		// sample.y <= 0.5的时候我们认为方向在左半边，否则就是在右半边
		if (sample.y > 0.5) phi += glm::pi<float>();

		float sinPhi = glm::sin(phi), cosPhi = glm::cos(phi);
		float A = square(cosPhi) / square(_alpha.x) + square(sinPhi) / square(_alpha.y);

		float cos2Theta = A * (sample.x - 1) / (sample.x * (A - 1) - A);
		cosTheta = std::sqrt(cos2Theta);
	}
	return GetUnitVectorUsingCos(cosTheta, phi);
}

float GGXDistribution::Pdf(const Vector3f& wo, const Vector3f& wh) const
{
	return D(wh) * std::abs(wh.y);
}

float GGXDistribution::lambda(const Vector3f& wh) const
{
	float cos2Theta = wh.y * wh.y;
	float sin2Theta = 1.f - cos2Theta;
	assert(sin2Theta >= 0.f);
	float tan2Theta = sin2Theta / cos2Theta;
	if (std::isinf(tan2Theta)) return 0.f;
	auto phi = getCosSinPhi(wh, std::sqrt(sin2Theta));
	float alpha2 = square(phi.x) * square(_alpha.x) + square(phi.y) * square(_alpha.y);
	return (-1 + std::sqrt(1 + alpha2 * tan2Theta)) / 2;
}

float BeckmannDistribution::RoughnessToAlpha(float roughness)
{
	roughness = std::max(roughness, 1e-3f);
	float x = std::log(roughness);
	return 1.62142f + 0.819955f * x + 0.1734f * x * x +
		0.0171201f * x * x * x + 0.000640711f * x * x * x * x;
}

BeckmannDistribution::BeckmannDistribution(Vector2f roughness)
	:_alpha(roughness.x, roughness.y)
{
	//printf("%lf, %lf\n", _alpha.x, _alpha.y);
}

BeckmannDistribution::~BeckmannDistribution()
{}

float BeckmannDistribution::D(const Vector3f& wh) const
{
	float cos2Theta = wh.y * wh.y;
	float sin2Theta = 1.f - cos2Theta;
	float tan2Theta = sin2Theta / cos2Theta;
	if (std::isinf(tan2Theta)) return 0.f;
	float cos4Theta = cos2Theta * cos2Theta;
	auto v = getCosSinPhi(wh, std::sqrt(sin2Theta));

	float bot = glm::pi<float>() * _alpha.x * _alpha.y * cos4Theta;
	float top = std::exp(-tan2Theta * (square(v.x) / square(_alpha.x) + square(v.y) / square(_alpha.y)));
	return top / bot;
}

float BeckmannDistribution::G(const Vector3f& wo, const Vector3f& wi) const
{
	return 1.f / (1.f + lambda(wo) + lambda(wi));
}

Vector3f BeckmannDistribution::Sample_wh(const Vector3f& wo, glm::vec2 sample) const
{
	float logSample = std::log(1.f - sample.x);
	if (std::isinf(logSample)) logSample = 0;
	float tan2Theta, phi;
	if (_alpha.x == _alpha.y)
	{
		tan2Theta = -square(_alpha.x) * logSample;
		phi = sample.y * glm::two_pi<float>();
	}
	else
	{
		phi = std::atan(_alpha.y / _alpha.x *
				   std::tan(glm::two_pi<float>() * sample.y + glm::half_pi<float>()));
		// sample.y <= 0.5的时候我们认为方向在左半边，否则就是在右半边
		if (sample.y > 0.5) phi += glm::pi<float>();
		float sinPhi = glm::sin(phi), cosPhi = glm::cos(phi);
		tan2Theta = -logSample /
			(cosPhi * cosPhi / square(_alpha.x) + sinPhi * sinPhi / square(_alpha.y));
	}

	float cosTheta = 1.f / std::sqrt(1.f + tan2Theta);
	return GetUnitVectorUsingCos(cosTheta, phi);
}

float BeckmannDistribution::Pdf(const Vector3f& wo, const Vector3f& wh) const
{
	return D(wh) * std::abs(wh.y);
}

float BeckmannDistribution::lambda(const Vector3f& wh) const
{
	float cosTheta = wh.y;
	float sinTheta = std::sqrt(1.f - cosTheta * cosTheta);
	float tanTheta = std::abs(sinTheta / cosTheta);
	if (std::isinf(tanTheta)) return 0.f;
	auto v = getCosSinPhi(wh, sinTheta);
	float alpha = std::sqrt(square(v.x) * square(_alpha.x) + square(v.y) * square(_alpha.y));
	float a = 1.f / (tanTheta * alpha);
	if (a >= 1.6f) return 0.f;
	return (1 - 1.259f * a + 0.396f * a * a) / (3.535f * a + 2.181f * a * a);
}
