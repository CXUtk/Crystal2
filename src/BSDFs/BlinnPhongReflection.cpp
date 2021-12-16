#include "BlinnPhongReflection.h"
#include <glm/gtx/transform.hpp>
#include <Core/Utils.h>
#include <Samplers/Sampler.h>
#include <mutex>
#include "BlinnPhongReflection.h"

crystal::BlinnPhongReflection::BlinnPhongReflection(glm::vec3 color, int k)
	: BxDF(BxDFType(BxDFType::BxDF_SPECULAR | BxDFType::BxDF_REFLECTION)), _albedo(color), _k(k)
{
	
}

glm::vec3 crystal::BlinnPhongReflection::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const
{
	auto V = glm::normalize(wOut + wIn);
	auto b = std::max(0.f, V.y);
	return _albedo * ((float)std::pow(b, _k) * (_k + 1) / glm::two_pi<float>());
	//return _albedo * ((_k + 1) / glm::two_pi<float>());
}

crystal::BlinnPhongReflection::~BlinnPhongReflection()
{}

static std::mt19937 mt;
glm::vec3 crystal::BlinnPhongReflection::SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const
{
	while (true)
	{
		float cosTheta = std::pow(sample.x, 1.0 / (_k + 1));
		float phi = glm::two_pi<float>() * sample.y;
		glm::vec3 N = GetUnitVectorUsingCos(cosTheta, phi);

		// Invalid normal
		if (glm::dot(N, wOut) < 0)
		{
			return glm::vec3(0);
			sample = glm::vec2(uniformRandomFloat(mt), uniformRandomFloat(mt));
			continue;
		}
		*wIn = glm::reflect(-wOut, N);
		if (wIn->y < 0)
		{
			return glm::vec3(0);
			sample = glm::vec2(uniformRandomFloat(mt), uniformRandomFloat(mt));
			continue;
		}
		*pdf = ((_k + 1) / glm::two_pi<float>()) * std::pow(cosTheta, _k);
		return DistributionFunction(wOut, *wIn);
	}
}

