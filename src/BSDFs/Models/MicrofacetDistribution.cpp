#include "MicrofacetDistribution.h"
#include <glm/gtx/transform.hpp>
#include <algorithm>

float MicrofacetDistribution::Pdf(glm::vec3 wi, glm::vec3 wh) const {
    return 0.f;
}

GGXDistribution::GGXDistribution(const glm::mat3& TNB, float alphaX, float alphaY) : _TNB(TNB), _alphaX(alphaX), _alphaY(alphaY) {
}

GGXDistribution::~GGXDistribution() {
}

float GGXDistribution::D(glm::vec3 wh) const {
    float cosTheta = std::max(0.f, wh.y);
    float cos2Theta = cosTheta * cosTheta;
    float sin2Theta = 1.f - cos2Theta;
    float tan2Theta = sin2Theta / cos2Theta;

    if (std::isinf(tan2Theta)) return 0.f;

    float cosPhi = (sin2Theta == 0.f) ? 1.f : glm::clamp(wh.x / std::sqrt(sin2Theta), -1.f, 1.f);
    float cos2Phi = cosPhi * cosPhi;
    float sin2Phi = 1.f - cos2Phi;

    float cos4Theta = cos2Theta * cos2Theta;
    float e = tan2Theta * (cos2Phi / (_alphaX * _alphaX) + sin2Phi / (_alphaY * _alphaY));
    return 1.f / (glm::pi<float>() * _alphaX * _alphaY * cos4Theta * (1 + e) * (1 + e));
}

float GGXDistribution::G(glm::vec3 wo, glm::vec3 wi) const {
    return 1.f / (1.f + lambda(wo) + lambda(wi));
}

glm::vec3 GGXDistribution::Sample_wh(glm::vec3 wo, glm::vec2 sample) const {
    return glm::vec3();
}

float GGXDistribution::Pdf(glm::vec3 wi, glm::vec3 wh) const {
    return 0.0f;
}

float GGXDistribution::lambda(glm::vec3 w) const {
    float cosTheta = std::max(0.f, w.y);
    float cos2Theta = cosTheta * cosTheta;
    float sin2Theta = 1.f - cos2Theta;
    float tan2Theta = sin2Theta / cos2Theta;

    if (std::isinf(tan2Theta)) return 0.f;

    float cosPhi = (sin2Theta == 0.f) ? 1.f : glm::clamp(w.x / std::sqrt(sin2Theta), -1.f, 1.f);
    float cos2Phi = cosPhi * cosPhi;
    float sin2Phi = 1.f - cos2Phi;

    float alpha2 = _alphaX * _alphaX * cos2Phi + _alphaY * _alphaY * sin2Phi;
    return 0.5f * (std::sqrt(1.f + alpha2 * tan2Theta) - 1.f);
}

float GGXDistribution::G1(glm::vec3 w) const {
    return 1.f / (1.f + lambda(w));
}



GGXRTDistribution::GGXRTDistribution(const glm::mat3& TNB, float roughness) : _TNB(TNB), _roughness(roughness) {
}

GGXRTDistribution::~GGXRTDistribution() {
}

float GGXRTDistribution::D(glm::vec3 wh) const {
    auto alpha = _roughness * _roughness;
    auto a2 = alpha * alpha;
    auto NdotH = std::max(wh.y, 0.f);
    auto NdotH2 = NdotH * NdotH;

    auto nom = a2;
    auto denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denom = glm::pi<float>() * denom * denom;

    return nom / std::max(denom, 0.0001f);
}

float GGXRTDistribution::G(glm::vec3 wo, glm::vec3 wi) const {
    return G1(wo) * G1(wi);
}

glm::vec2 Hammersley(uint32_t i, uint32_t N) { // 0-1
    uint32_t bits = (i << 16u) | (i >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    float rdi = float(bits) * 2.3283064365386963e-10;
    return { float(i) / float(N), rdi };
}


glm::vec3 GGXRTDistribution::Sample_wh(glm::vec3 wo, glm::vec2 sample) const {
    float a = _roughness * _roughness;
    auto thetaM = std::atan(a * std::sqrt(sample.x) / std::sqrt(1 - sample.x));
    auto phiH = glm::two_pi<float>() * sample.y;
    auto r = std::sin(thetaM);
    auto dir = glm::vec3(std::cos(phiH) * r, std::cos(thetaM), std::sin(phiH) * r);
    return dir;
}

float GGXRTDistribution::Pdf(glm::vec3 wi, glm::vec3 wh) const {
    auto NdotH = std::max(wh.y, 0.f);
    auto IdotH = std::max(glm::dot(wi, wh), 0.f);
    return D(wh) * NdotH / (4.f * IdotH);
}

float GGXRTDistribution::G1(glm::vec3 w) const {
    auto k = (_roughness * _roughness) / 2.0f;
    auto NdotH = std::max(w.y, 0.f);
    return NdotH / (NdotH * (1.0f - k) + k);
}
