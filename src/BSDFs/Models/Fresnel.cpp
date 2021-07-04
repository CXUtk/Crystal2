#include "Fresnel.h"
#include <algorithm>

float FresnelDielectric::Eval(float cosThetaI) const {
    auto sinThetaI = std::sqrt(std::max(0.f, 1.f - cosThetaI * cosThetaI));
    auto sinThetaT = _etaA / _etaB * sinThetaI;
    if (sinThetaT >= 1.f) return 1.f;

    auto cosThetaT = std::sqrt(std::max(0.f, 1.f - sinThetaT * sinThetaT));
    auto Rparl = ((_etaB * cosThetaI) - (_etaA * cosThetaT)) /
        ((_etaB * cosThetaI) + (_etaA * cosThetaT));
    auto Rperp = ((_etaA * cosThetaI) - (_etaB * cosThetaT)) /
        ((_etaA * cosThetaI) + (_etaB * cosThetaT));
    return (Rparl * Rparl + Rperp * Rperp) / 2;
}
