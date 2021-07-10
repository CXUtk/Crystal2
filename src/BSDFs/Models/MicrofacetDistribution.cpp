#include "MicrofacetDistribution.h"
#include <glm/gtx/transform.hpp>
#include <algorithm>

GGXDistribution::GGXDistribution(const glm::mat3& TNB, float alphaX, float alphaY) : _TNB(TNB), _alphaX(alphaX), _alphaY(alphaY) {
    _invTNB = glm::inverse(TNB);
}

GGXDistribution::~GGXDistribution() {
}

float GGXDistribution::D(glm::vec3 wh) const {
    auto Wh = _invTNB * wh;

    float cosTheta = std::max(0.f, Wh.y);
    float cos2Theta = cosTheta * cosTheta;
    float sin2Theta = 1.f - cos2Theta;
    float tan2Theta = sin2Theta / cos2Theta;

    if (std::isinf(tan2Theta)) return 0.f;

    float cosPhi = (sin2Theta == 0.f) ? 1.f : glm::clamp(Wh.x / std::sqrt(sin2Theta), -1.f, 1.f);
    float cos2Phi = cosPhi * cosPhi;
    float sin2Phi = 1.f - cos2Phi;

    float cos4Theta = cos2Theta * cos2Theta;
    float e = tan2Theta * (cos2Phi / (_alphaX * _alphaX) + sin2Phi / (_alphaY * _alphaY));
    return 1.f / (glm::pi<float>() * _alphaX * _alphaY * cos4Theta * (1 + e) * (1 + e));
}

float GGXDistribution::Lambda(glm::vec3 w) const {
    auto Wh = _invTNB * w;

    float cosTheta = std::max(0.f, Wh.y);
    float cos2Theta = cosTheta * cosTheta;
    float sin2Theta = 1.f - cos2Theta;
    float tan2Theta = sin2Theta / cos2Theta;

    if (std::isinf(tan2Theta)) return 0.f;

    float cosPhi = (sin2Theta == 0.f) ? 1.f : glm::clamp(Wh.x / std::sqrt(sin2Theta), -1.f, 1.f);
    float cos2Phi = cosPhi * cosPhi;
    float sin2Phi = 1.f - cos2Phi;

    float alpha2 = _alphaX * _alphaX * cos2Phi + _alphaY * _alphaY * sin2Phi;
    return 0.5f * (std::sqrt(1.f + alpha2 * tan2Theta) - 1.f);
}
