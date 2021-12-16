﻿#include "SpecularReflection.h"


SpecularReflection::SpecularReflection(glm::vec3 color, float etaA, float etaB, const std::shared_ptr<Fresnel>& fresnel) :
    BxDF(BxDFType(BxDFType::BxDF_SPECULAR | BxDFType::BxDF_REFLECTION)), _albedo(color), _etaA(etaA), _etaB(etaB),
    _fresnel(fresnel) {
}

SpecularReflection::~SpecularReflection() {
}

glm::vec3 SpecularReflection::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const {
    return glm::vec3(0);
}

glm::vec3 SpecularReflection::SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const {
    *sampledType = GetType();
    *wIn = glm::reflect(-wOut, Vector3f(0, 1, 0));
    *pdf = 1.0f;
    return _albedo * _fresnel->Eval(_etaA, _etaB, wOut.y);
}
