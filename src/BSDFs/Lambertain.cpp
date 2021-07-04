#include "Lambertain.h"
#include <glm/gtx/transform.hpp>
#include <Core/Utils.h>

Lambertain::Lambertain(glm::vec3 color, glm::mat3 TNB) 
    : BxDF(BxDFType(BxDFType::BxDF_DIFFUSE | BxDFType::BxDF_REFLECTION)), _albedo(color), _TNB(TNB) {
}

Lambertain::~Lambertain() {
}

glm::vec3 Lambertain::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const {
    return _albedo / glm::pi<float>();
}

glm::vec3 Lambertain::SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const {
    *sampledType = GetType();
    auto dir = NextCosineUnitHemiSphere(sample, *pdf);
    *wIn = _TNB * dir;
    return DistributionFunction(wOut, *wIn);
}
