#include "Prototype.h"
#include <Materials/Material.h>
#include <SJson/SJson.h>


std::unique_ptr<Prototype> Prototype::CreatePrototype(JsonNode_CPTR pNode,
    const Scene* scene)
{
    std::shared_ptr<crystal::AreaLight> areaLight = nullptr;
    std::shared_ptr<Material> material = nullptr;
    if (pNode->HasMember("Material"))
    {
        material = Material::CreateMaterial(pNode->GetMember("Material"), scene);
    }
    if (pNode->HasMember("AreaLight"))
    {

    }
    return std::make_unique<Prototype>(areaLight, material);
}

Prototype::Prototype(const std::shared_ptr<crystal::AreaLight>& light, const std::shared_ptr<Material>& material)
    : _light(light), _material(material) {
}

Prototype::~Prototype() {
}

void Prototype::ComputeScatteringFunctions(SurfaceInteraction& isec, glm::vec3 dir, bool fromCamera) const {
    _material->ComputeScatteringFunctions(isec, fromCamera);
}
