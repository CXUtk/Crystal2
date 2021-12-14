#include "Material.h"
#include <SJson/SJson.h>
#include <Loaders/JsonLoader.h>
#include <Core/Scene.h>


#include "DiffuseMaterial.h"
#include "MirrorMaterial.h"
#include "Glass.h"
#include "MicrofacetMaterial.h"
#include "PhongMaterial.h"
#include <Textures/Texture.h>

static std::vector<std::shared_ptr<PureTexture>> textures;

std::shared_ptr<Material> Material::CreateMaterial(JsonNode_CPTR pNode, const Scene* scene) {
    auto type = pNode->GetMember("Type")->GetString();
    if (type == "Diffuse") {
        if (pNode->HasMember("Kd"))
        {
            auto kdTexture = pNode->GetMember("Kd")->GetString();
            return std::make_shared<DiffuseMaterial>(scene->GetTextureByName(kdTexture));
        }
        else
        {
            auto color = loader::parse_vec3(pNode->GetMember("Color"));
            textures.push_back(std::make_shared<PureTexture>(color));
            return std::make_shared<DiffuseMaterial>(ptr(textures.back()));
        }
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
    else if (type == "Phong")
    {
        auto k = pNode->GetMember("K")->GetInt();
        if (pNode->HasMember("Kd"))
        {
            auto kdTexture = pNode->GetMember("Kd")->GetString();
            return std::make_shared<crystal::PhongMaterial>(scene->GetTextureByName(kdTexture), k);
        }
        else
        {
            auto color = loader::parse_vec3(pNode->GetMember("Color"));
            textures.push_back(std::make_shared<PureTexture>(color));
            return std::make_shared<crystal::PhongMaterial>(ptr(textures.back()), k);
        }
    }
    else {
        throw std::exception("Invalid material name");
    }
}
