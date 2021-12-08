#include "PhongReflection.h"
#include <glm/gtx/transform.hpp>
#include <Core/Utils.h>
#include <Samplers/Sampler.h>
#include <mutex>

PhongReflection::PhongReflection(glm::vec3 color, const glm::mat3& TNB, int k)
	: BxDF(BxDFType(BxDFType::BxDF_SPECULAR | BxDFType::BxDF_REFLECTION)), _albedo(color), _TNB(TNB), _k(k)
{
	
}

glm::vec3 PhongReflection::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const
{
	auto V = glm::normalize(wOut + wIn);
	auto b = std::max(0.f, glm::dot(V, _TNB[1]));
	return _albedo * ((float)std::pow(b, _k) * (_k + 1) / glm::two_pi<float>());
	//return _albedo * ((_k + 1) / glm::two_pi<float>());
}

PhongReflection::~PhongReflection()
{}

static std::mt19937 mt;
static bool f = true;
glm::vec3 PhongReflection::SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const
{
	*sampledType = GetType();
	while (true)
	{
		float cosTheta = std::pow(sample.x, 1.0 / (_k + 1));
		float phi = glm::two_pi<float>() * sample.y;
		glm::vec3 dir = GetUnitVectorUsingCos(cosTheta, phi);
		glm::vec3 N = _TNB * dir;


		// Invalid normal
		if (glm::dot(N, wOut) < 0)
		{
			//return glm::vec3(0);
			sample = glm::vec2(uniformRandomFloat(mt), uniformRandomFloat(mt));
			continue;
		}
		*wIn = glm::reflect(-wOut, N);
		if (glm::dot(_TNB[1], *wIn) < 0)
		{
			//return glm::vec3(0);
			sample = glm::vec2(uniformRandomFloat(mt), uniformRandomFloat(mt));
			continue;
		}
		*pdf = ((_k + 1) / glm::two_pi<float>()) * std::pow(cosTheta, _k);
		if (f)
		{
			printf("%lf\n", *pdf);
			f = false;
		}
		return DistributionFunction(wOut, *wIn);
	}
}