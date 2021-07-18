#include "MicrofacetReflection.h"
#include <Core/Utils.h>



MicrofacetReflection::MicrofacetReflection(glm::vec3 color, const glm::mat3& TNB, float etaA, float etaB, const std::shared_ptr<Fresnel>& fresnel,
    const std::shared_ptr<MicrofacetDistribution>& distribution) : BxDF(BxDFType(BxDFType::BxDF_REFLECTION | BxDFType::BxDF_GLOSSY)),
    _R(color), _TNB(TNB), _etaA(etaA), _etaB(etaB), _fresnel(fresnel), _microDistribution(distribution) {
    _invTNB = glm::transpose(_TNB);

}

MicrofacetReflection::~MicrofacetReflection() {
}

glm::vec3 MicrofacetReflection::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const {
    auto V = _invTNB * wOut;
    auto I = _invTNB * wIn;
    auto H = _invTNB * glm::normalize(wOut + wIn);
    if (I.y <= 0 || V.y <= 0) return glm::vec3(0);

    auto F = _fresnel->Eval(_etaA, _etaB, std::max(0.f, glm::dot(V, H)));
    auto G = _microDistribution->G(V, I);
    auto D = _microDistribution->D(H);
    return glm::vec3(1.f) * (D * G * F / (4 * std::max(0.f, I.y) * std::max(0.f, V.y)));
}



glm::vec3 MicrofacetReflection::SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const {
    *sampledType = BxDFType(BxDFType::BxDF_GLOSSY | BxDFType::BxDF_REFLECTION);
    auto H = _microDistribution->Sample_wh(wOut, sample);
    auto V = _invTNB * wOut;
    if (glm::dot(V, H) <= 0) return glm::vec3(0.f);
    auto L = glm::reflect(-V, H);
    *pdf = _microDistribution->Pdf(L, H);
    *wIn = _TNB * L;
    if (glm::isnan(*pdf)) {
        assert(false);
    }
    return DistributionFunction(wOut, *wIn);
}
