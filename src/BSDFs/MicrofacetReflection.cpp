#include "MicrofacetReflection.h"
#include <Core/Utils.h>



MicrofacetReflection::MicrofacetReflection(glm::vec3 color, const glm::mat3& TNB, const std::shared_ptr<Fresnel>& fresnel,
    const std::shared_ptr<MicrofacetDistribution>& distribution) : BxDF(BxDFType(BxDFType::BxDF_REFLECTION | BxDFType::BxDF_GLOSSY)),
    _R(color), _TNB(TNB), _fresnel(fresnel), _microDistribution(distribution) {

}

MicrofacetReflection::~MicrofacetReflection() {
}

glm::vec3 MicrofacetReflection::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const {
    auto cosThetaO = glm::dot(wOut, _TNB[1]);
    auto cosThetaI = glm::dot(wIn, _TNB[1]);
    if (cosThetaI == 0 || cosThetaO == 0) return glm::vec3(0);
    auto wh = glm::normalize(wOut + wIn);
    auto F = _fresnel->Eval(glm::dot(wOut, wh));
    auto G = _microDistribution->G(wOut, wIn);
    auto D = _microDistribution->D(wh);
    return _R * (D * G * F / (4 * cosThetaI * cosThetaO));
}



glm::vec3 MicrofacetReflection::SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const {
    *sampledType = BxDFType(BxDFType::BxDF_GLOSSY | BxDFType::BxDF_REFLECTION);
    auto dir = NextCosineUnitHemiSphere(sample, *pdf);
    *wIn = glm::reflect(-wOut, _TNB * dir);

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
