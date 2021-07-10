#include "Prototype.h"
#include <Materials/Material.h>
#include <SJson/SJson.h>


std::shared_ptr<Prototype> Prototype::CreatePrototype(const std::shared_ptr<SJson::SJsonNode>& node, const Scene* scene) {
    std::shared_ptr<AreaLight> light = nullptr;
    std::shared_ptr<Material> material = nullptr;
    if (node->HasMember("Material")) {
        material = Material::CreateMaterial(node->GetMember("Material"), scene);
    }

    return std::make_shared<Prototype>(nullptr, material);
}

Prototype::Prototype(const std::shared_ptr<AreaLight>& light, const std::shared_ptr<Material>& material)
    : _light(light), _material(material) {
}

Prototype::~Prototype() {
}

std::shared_ptr<BSDF> Prototype::ComputeScatteringFunctions(SurfaceInteraction& isec, glm::vec3 dir, bool fromCamera) const {
    return _material->ComputeScatteringFunctions(isec, fromCamera);
}
