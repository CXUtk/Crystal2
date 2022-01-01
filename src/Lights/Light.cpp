#include "Light.h"
#include "AreaLight.h"
#include "DiffusedAreaLight.h"
#include <SJson/SJson.h>
#include <Loaders/JsonLoader.h>


#include "PointLight.h"

namespace crystal
{
	std::shared_ptr<Light> Light::CreateLight(JsonNode_CPTR pNode, const Scene* scene)
	{
		auto lightType = pNode->GetMember("LightType")->GetString();
		if (lightType == "Point")
		{
			auto pos = loader::parse_vec3(pNode->GetMember("Position"));
			auto flux = loader::parse_vec3(pNode->GetMember("Flux"));
			return std::make_shared<PointLight>(pos, flux);
		}
		else
		{
			throw std::exception("Invalid light name");
		}
	}
	std::shared_ptr<AreaLight> Light::CreateAreaLight(JsonNode_CPTR pNode, const Shape* shape, const Scene* scene)
	{
		auto lightType = pNode->GetMember("Type")->GetString();
		if (lightType == "Diffused")
		{
			auto flux = loader::parse_vec3(pNode->GetMember("Lemit"));
			auto numSamples = pNode->GetMember("Samples")->GetInt();
			return std::make_shared<DiffusedAreaLight>(shape, flux, numSamples);
		}
		else
		{
			throw std::exception("Invalid area light name");
		}
	}
}
