#include "Light.h"
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
			throw std::exception("Invalid material name");
		}
	}
}
