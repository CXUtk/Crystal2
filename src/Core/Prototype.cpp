#include "Prototype.h"
#include <Materials/Material.h>
#include <SJson/SJson.h>


std::unique_ptr<Prototype> Prototype::CreatePrototype(JsonNode_CPTR pNode,
    const Scene* scene)
{
    std::shared_ptr<AreaLight> light = nullptr;
    std::shared_ptr<Material> material = nullptr;
    if (pNode->HasMember("Material"))
    {
        material = Material::CreateMaterial(pNode->GetMember("Material"), scene);
    }
    return std::make_unique<Prototype>(nullptr, material);
}

Prototype::Prototype(const std::shared_ptr<AreaLight>& light, const std::shared_ptr<Material>& material)
    : _light(light), _material(material) {
}

Prototype::~Prototype() {
}

std::shared_ptr<BSDF> Prototype::ComputeScatteringFunctions(SurfaceInteraction& isec, glm::vec3 dir, bool fromCamera) const {
    return _material->ComputeScatteringFunctions(isec, fromCamera);
}
