#include "Fresnel.h"
#include <algorithm>


glm::vec3 FresnelDielectric::Eval(float etaA, float etaB, float cosThetaI) const {
    auto sinThetaI = std::sqrt(std::max(0.f, 1.f - cosThetaI * cosThetaI));
    auto sinThetaT = etaA / etaB * sinThetaI;
    if (sinThetaT >= 1.f) return glm::vec3(1.f);

    auto cosThetaT = std::sqrt(std::max(0.f, 1.f - sinThetaT * sinThetaT));
    auto Rparl = ((etaB * cosThetaI) - (etaA * cosThetaT)) /
        ((etaB * cosThetaI) + (etaA * cosThetaT));
    auto Rperp = ((etaA * cosThetaI) - (etaB * cosThetaT)) /
        ((etaA * cosThetaI) + (etaB * cosThetaT));
    return glm::vec3((Rparl * Rparl + Rperp * Rperp) / 2);
}

glm::vec3 FresnelSchlick::Eval(float etaA, float etaB, float cosThetaI) const {
    auto sinThetaI = std::sqrt(std::max(0.f, 1.f - cosThetaI * cosThetaI));
    auto sinThetaT = etaA / etaB * sinThetaI;
    if (sinThetaT >= 1.f) return glm::vec3(1.f);

    return _r0 + (glm::vec3(1.f) - _r0) * std::pow(1.f - cosThetaI, 5.f);
}
