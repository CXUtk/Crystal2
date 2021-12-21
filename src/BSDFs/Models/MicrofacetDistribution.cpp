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

//float MicrofacetDistribution::Pdf(const Vector3f& wi, const Vector3f& wh) const
//{
//	return 0.f;
//}
//
//GGXDistribution::GGXDistribution(const glm::mat3& TNB, float alphaX, float alphaY) : _TNB(TNB), _alphaX(alphaX), _alphaY(alphaY)
//{}
//
//GGXDistribution::~GGXDistribution()
//{}
//
//float GGXDistribution::D(const Vector3f& wh) const
//{
//	float cosTheta = std::max(0.f, wh.y);
//	float cos2Theta = cosTheta * cosTheta;
//	float sin2Theta = 1.f - cos2Theta;
//	float tan2Theta = sin2Theta / cos2Theta;
//
//	if (std::isinf(tan2Theta)) return 0.f;
//
//	float cosPhi = (sin2Theta == 0.f) ? 1.f : glm::clamp(wh.x / std::sqrt(sin2Theta), -1.f, 1.f);
//	float cos2Phi = cosPhi * cosPhi;
//	float sin2Phi = 1.f - cos2Phi;
//
//	float cos4Theta = cos2Theta * cos2Theta;
//	float e = tan2Theta * (cos2Phi / (_alphaX * _alphaX) + sin2Phi / (_alphaY * _alphaY));
//	return 1.f / (glm::pi<float>() * _alphaX * _alphaY * cos4Theta * (1 + e) * (1 + e));
//}
//
//float GGXDistribution::G(const Vector3f& wo, const Vector3f& wi) const
//{
//	return 1.f / (1.f + lambda(wo) + lambda(wi));
//}
//
//const Vector3f& GGXDistribution::Sample_wh(const Vector3f& wo, glm::vec2 sample) const
//{
//	return const Vector3f&();
//}
//
//float GGXDistribution::Pdf(const Vector3f& wi, const Vector3f& wh) const
//{
//	return 0.0f;
//}
//
//float GGXDistribution::lambda(const Vector3f& w) const
//{
//	float cosTheta = std::max(0.f, w.y);
//	float cos2Theta = cosTheta * cosTheta;
//	float sin2Theta = 1.f - cos2Theta;
//	float tan2Theta = sin2Theta / cos2Theta;
//
//	if (std::isinf(tan2Theta)) return 0.f;
//
//	float cosPhi = (sin2Theta == 0.f) ? 1.f : glm::clamp(w.x / std::sqrt(sin2Theta), -1.f, 1.f);
//	float cos2Phi = cosPhi * cosPhi;
//	float sin2Phi = 1.f - cos2Phi;
//
//	float alpha2 = _alphaX * _alphaX * cos2Phi + _alphaY * _alphaY * sin2Phi;
//	return 0.5f * (std::sqrt(1.f + alpha2 * tan2Theta) - 1.f);
//}
//
//float GGXDistribution::G1(const Vector3f& w) const
//{
//	return 1.f / (1.f + lambda(w));
//}
//
//
//
//GGXRTDistribution::GGXRTDistribution(const glm::mat3& TNB, float roughness) : _TNB(TNB), _roughness(roughness)
//{}
//
//GGXRTDistribution::~GGXRTDistribution()
//{}
//
//float GGXRTDistribution::D(const Vector3f& wh) const
//{
//	auto alpha = _roughness * _roughness;
//	auto a2 = alpha * alpha;
//	auto NdotH = std::max(wh.y, 0.f);
//	auto NdotH2 = NdotH * NdotH;
//
//	auto nom = a2;
//	auto denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
//	denom = glm::pi<float>() * denom * denom;
//
//	return nom / denom;
//}
//
//float GGXRTDistribution::G(const Vector3f& wo, const Vector3f& wi) const
//{
//	return G1(wo) * G1(wi);
//}
//
//glm::vec2 Hammersley(uint32_t i, uint32_t N)
//{ // 0-1
//	uint32_t bits = (i << 16u) | (i >> 16u);
//	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
//	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
//	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
//	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
//	float rdi = float(bits) * 2.3283064365386963e-10;
//	return { float(i) / float(N), rdi };
//}
//
//
//const Vector3f& GGXRTDistribution::Sample_wh(const Vector3f& wo, glm::vec2 sample) const
//{
//	float a = _roughness * _roughness;
//	auto thetaM = std::atan(a * std::sqrt(sample.x) / std::sqrt(1 - sample.x));
//	auto phiH = glm::two_pi<float>() * sample.y;
//	auto r = std::sin(thetaM);
//	auto dir = const Vector3f&(std::cos(phiH) * r, std::cos(thetaM), std::sin(phiH) * r);
//	return dir;
//}
//
//float GGXRTDistribution::Pdf(const Vector3f& wi, const Vector3f& wh) const
//{
//	auto NdotH = std::max(wh.y, 0.f);
//	auto IdotH = std::max(glm::dot(wi, wh), 0.f);
//	return D(wh) * NdotH / (4.f * IdotH);
//}
//
//float GGXRTDistribution::G1(const Vector3f& w) const
//{
//	auto k = (_roughness * _roughness) / 2.0f;
//	auto NdotH = std::max(w.y, 0.f);
//	return NdotH / (NdotH * (1.0f - k) + k);
//}

BeckmannDistribution::BeckmannDistribution(float alphaX, float alphaY)
	: _alpha(alphaX, alphaY)
{}

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
	float tan2Theta = -square(_alpha.x) * logSample;
	float phi = sample.y * glm::two_pi<float>();
	float cosTheta = 1.f / std::sqrt(1.f + tan2Theta);
	return GetUnitVectorUsingCos(cosTheta, phi);
}

float BeckmannDistribution::Pdf(const Vector3f& wo, const Vector3f& wh) const
{
	return D(wh) * std::max(0.f, wh.y);
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
