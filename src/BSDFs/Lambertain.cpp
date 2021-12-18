#include "Lambertain.h"
#include <glm/gtx/transform.hpp>
#include <Core/Utils.h>

Lambertain::Lambertain(glm::vec3 color)
	: BxDF(BxDFType(BxDFType::BxDF_DIFFUSE | BxDFType::BxDF_REFLECTION)), _albedo(color)
{}

Lambertain::~Lambertain()
{}

glm::vec3 Lambertain::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const
{
	return _albedo / glm::pi<float>();
}

glm::vec3 Lambertain::SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const
{
	*wIn = NextCosineUnitHemiSphere(sample, pdf);
	return DistributionFunction(wOut, *wIn);
}
