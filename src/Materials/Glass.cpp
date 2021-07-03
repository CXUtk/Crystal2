#include <Core/SurfaceInteraction.h>
#include <BSDFs/SpecularReflection.h>
#include <BSDFs/SpecularTransmission.h>
#include "Glass.h"

Glass::Glass(glm::vec3 color, float eta) : _color(color), _eta(eta) {
}

Glass::~Glass() {
}


static float Fresnel(glm::vec3 N, glm::vec3 wo, glm::vec3 T, float etaA, float etaB) {
    auto cosI = glm::dot(N, wo);
    auto cosT = glm::dot(-N, T);
    float r1 = (etaB * cosI - etaA * cosT) / (etaB * cosI + etaA * cosT);
    float r2 = (etaA * cosI - etaB * cosT) / (etaA * cosI + etaB * cosT);
    return 0.5 * (r1 * r1 + r2 * r2);
}


static bool refract(glm::vec3 wo, glm::vec3 N, float etaA, float etaB, glm::vec3& wt) {
    float eta = etaA / etaB;
    auto cosThetaI = glm::dot(wo, N);
    auto sin2ThetaI = std::max(0.f, 1.f - cosThetaI * cosThetaI);
    auto sin2ThetaT = eta * eta * sin2ThetaI;
    auto cosThetaT = std::sqrt(1.f - sin2ThetaT);
    if (sin2ThetaT > 1.f) return false;
    auto t = glm::dot(wo, N);
    wt = eta * (-wo) + (eta * glm::dot(wo, N) - cosThetaT) * N;
    return true;
}

std::shared_ptr<BSDF> Glass::ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera) const {
    auto N = glm::normalize(isec.GetNormal());
    float etaA = 1.0f, etaB = _eta;
    if (!isec.IsFrontFace()) std::swap(etaA, etaB);

    auto bsdf = std::make_shared<BSDF>(&isec);
    glm::vec3 tdir;
    if (refract(-isec.GetHitDir(), N, etaA, etaB, tdir)) {
        float f = Fresnel(N, -isec.GetHitDir(), tdir, etaA, etaB);
        bsdf->AddBxDF(std::make_shared<SpecularReflection>(_color, N), f);
        bsdf->AddBxDF(std::make_shared<SpecularTransmission>(_color, N, etaA, etaB), 1.f - f);
    }
    else {
        bsdf->AddBxDF(std::make_shared<SpecularReflection>(_color, N), 1.f);
    }
    return bsdf;
}
