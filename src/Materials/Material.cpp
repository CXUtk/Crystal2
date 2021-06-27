#include "Material.h"
#include <SJson/SJson.h>
#include <Loaders/JsonLoader.h>
#include "DiffuseMaterial.h"
#include "MirrorMaterial.h"

std::shared_ptr<Material> Material::CreateMaterial(const std::shared_ptr<SJson::SJsonNode>& node) {
    auto type = node->GetMember("Type")->GetString();
    if (type == "Diffuse") {
        auto color = loader::parse_vec3(node->GetMember("Color"));
        return std::make_shared<DiffuseMaterial>(color);
    }
    else if (type == "Mirror") {
        auto color = loader::parse_vec3(node->GetMember("Color"));
        return std::make_shared<MirrorMaterial>(color);
    }
    else {
        throw std::exception("Invalid material name");
    }
    return std::shared_ptr<Material>();
}
