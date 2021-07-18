#include "MicrofacetReflection.h"
#include <Core/Utils.h>



MicrofacetReflection::MicrofacetReflection(glm::vec3 color, const glm::mat3& TNB, float etaA, float etaB, const std::shared_ptr<Fresnel>& fresnel,
    const std::shared_ptr<MicrofacetDistribution>& distribution) : BxDF(BxDFType(BxDFType::BxDF_REFLECTION | BxDFType::BxDF_GLOSSY)),
    _R(color), _TNB(TNB), _etaA(etaA), _etaB(etaB), _fresnel(fresnel), _microDistribution(distribution) {
    _invTNB = glm::inverse(_TNB);

}

MicrofacetReflection::~MicrofacetReflection() {
}

glm::vec3 MicrofacetReflection::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const {
    auto V = _invTNB * wOut;
    auto I = _invTNB * wIn;
    auto H = _invTNB * glm::normalize(wOut + wIn);
    if (I.y == 0 || V.y == 0) return glm::vec3(0);

    auto F = _fresnel->Eval(_etaA, _etaB, std::max(0.f, glm::dot(V, H)));
    auto G = _microDistribution->G(V, I);
    auto D = _microDistribution->D(H);
    return glm::vec3(1.f) * (D * G * F / (4 * I.y * V.y));
}



glm::vec3 MicrofacetReflection::SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const {
    *sampledType = BxDFType(BxDFType::BxDF_GLOSSY | BxDFType::BxDF_REFLECTION);
    auto H = _microDistribution->Sample_wh(wOut, sample);
    *pdf = _microDistribution->Pdf(wOut, H);
    auto dir = _TNB * H;
    if (glm::dot(wOut, dir) <= 0) return glm::vec3(0.f);
    *wIn = glm::reflect(-wOut, dir);

    //auto cosThetaO = glm::dot(wOut, _TNB[1]);
    //auto cosThetaI = glm::dot(*wIn, _TNB[1]);
    //if (cosThetaI == 0 || cosThetaO == 0) return glm::vec3(0);
    //auto wh = glm::normalize(wOut + *wIn);
    //auto F = _fresnel->Eval(glm::dot(wOut, wh));
    //auto G = _microDistribution->G(wOut, *wIn);
    //auto D = _microDistribution->D(wh);
    //return _R * (D * G * F / (4 * cosThetaI * cosThetaO));
    return DistributionFunction(wOut, *wIn);
}
