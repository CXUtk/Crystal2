#include "Material.h"
#include <SJson/SJson.h>
#include <Loaders/JsonLoader.h>
#include <Core/Scene.h>


#include "DiffuseMaterial.h"
#include "MirrorMaterial.h"
#include "Glass.h"
#include "MicrofacetMaterial.h"

std::shared_ptr<Material> Material::CreateMaterial(JsonNode_CPTR pNode, const Scene* scene) {
    auto type = pNode->GetMember("Type")->GetString();
    if (type == "Diffuse") {
        auto kdTexture = pNode->GetMember("Kd")->GetString();
        return std::make_shared<DiffuseMaterial>(scene->GetTextureByName(kdTexture));
    }
    else if (type == "Mirror") {
        auto color = loader::parse_vec3(pNode->GetMember("Color"));
        return std::make_shared<MirrorMaterial>(color);
    }
    else if (type == "Glass") {
        auto color = loader::parse_vec3(pNode->GetMember("Color"));
        auto eta = pNode->GetMember("IOR")->GetFloat();
        return std::make_shared<Glass>(color, eta);
    }
    else if (type == "Micro") {
        auto color = loader::parse_vec3(pNode->GetMember("Color"));
        auto roughness = pNode->GetMember("Roughness")->GetFloat();
        return std::make_shared<MicrofacetMaterial>(color, 1.5f, roughness);
    }
    else {
        throw std::exception("Invalid material name");
    }
    return std::shared_ptr<Material>();
}
