#include "SpecularReflection.h"


SpecularReflection::SpecularReflection(glm::vec3 color, glm::vec3 N, const std::shared_ptr<Fresnel>& fresnel) :
    BxDF(BxDFType(BxDFType::BxDF_SPECULAR | BxDFType::BxDF_REFLECTION)), _albedo(color), _normal(N),
    _fresnel(fresnel) {
}

SpecularReflection::~SpecularReflection() {
}

glm::vec3 SpecularReflection::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const {
    return glm::vec3(0);
}

glm::vec3 SpecularReflection::SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf) const {
    *wIn = glm::reflect(-wOut, _normal);
    *pdf = 1.0f;
    return _albedo * _fresnel->Eval(glm::dot(wOut, _normal));
}
