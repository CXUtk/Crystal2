#include "SpecularTransmission.h"

SpecularTransmission::SpecularTransmission(glm::vec3 color, glm::vec3 N, const std::shared_ptr<Fresnel>& fresnel,
    float etaA, float etaB)
    : BxDF(BxDFType(BxDFType::BxDF_SPECULAR | BxDFType::BxDF_TRANSMISSION)),
    _albedo(color), _normal(N), _fresnel(fresnel), _etaA(etaA), _etaB(etaB) {

}

SpecularTransmission::~SpecularTransmission() {
}

glm::vec3 SpecularTransmission::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const {
    return glm::vec3(0);
}

static bool refract(glm::vec3 wo, glm::vec3 N, float etaA, float etaB, glm::vec3& wt) {
    float eta = etaA / etaB;
    auto cosThetaI = glm::dot(wo, N);
    auto sin2ThetaI = std::max(0.f, 1.f - cosThetaI * cosThetaI);
    auto sin2ThetaT = eta * eta * sin2ThetaI;
    if (sin2ThetaT > 1.f) return false;
    auto cosThetaT = std::sqrt(1.f - sin2ThetaT);
    auto t = glm::dot(wo, N);
    wt = eta * (-wo) + (eta * glm::dot(wo, N) - cosThetaT) * N;
    return true;
}


glm::vec3 SpecularTransmission::SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf) const {
    glm::vec3 transDir;
    *pdf = 1.0f;
    if (refract(wOut, _normal, _etaA, _etaB, transDir)) {
        *wIn = transDir;
    }
    else {
        return glm::vec3(0);
    }
    return _albedo * (1.f - _fresnel->Eval(glm::dot(wOut, _normal)));
}
