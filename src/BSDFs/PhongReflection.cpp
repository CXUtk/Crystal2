#include "PhongReflection.h"
#include <glm/gtx/transform.hpp>
#include <Core/Utils.h>
#include <Samplers/Sampler.h>

PhongReflection::PhongReflection(glm::vec3 color, const glm::mat3& TNB, int k)
	: BxDF(BxDFType(BxDFType::BxDF_SPECULAR | BxDFType::BxDF_REFLECTION)), _albedo(color), _TNB(TNB), _k(k)
{
	
}

glm::vec3 PhongReflection::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const
{
	float thetaO = std::acos(glm::dot(wOut, _TNB[1]));


	auto V = glm::normalize(wOut + wIn);
	return _albedo * std::pow(glm::dot(V, _TNB[1]), (float)_k) * ((_k + 1) / glm::two_pi<float>());
	return _albedo * ((_k + 1) / glm::two_pi<float>());
}

PhongReflection::~PhongReflection()
{}

static std::mt19937 mt;
glm::vec3 PhongReflection::SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const
{
	while (true)
	{
		float cosTheta = std::pow(sample.x, 1.0 / (_k + 1));
		float phi = glm::two_pi<float>() * sample.y;
		glm::vec3 dir = GetUnitVectorUsingCos(cosTheta, phi);
		glm::vec3 N = _TNB * dir;


		// Invalid normal
		if (glm::dot(N, wOut) <= 0)
		{
			sample = glm::vec2(uniformRandomFloat(mt), uniformRandomFloat(mt));
			continue;
		}
		*wIn = glm::reflect(-wOut, N);
		if (glm::dot(_TNB[1], *wIn) <= 0)
		{
			sample = glm::vec2(uniformRandomFloat(mt), uniformRandomFloat(mt));
			continue;
		}
		*pdf = ((_k + 1) / glm::two_pi<float>()) * std::pow(cosTheta, _k);
		break;
	}
	return DistributionFunction(wOut, *wIn);
}