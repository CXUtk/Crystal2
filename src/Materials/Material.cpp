#include "Material.h"
#include <SJson/SJson.h>
#include <Loaders/JsonLoader.h>
#include <Core/Scene.h>


#include "DiffuseMaterial.h"
#include "MirrorMaterial.h"
#include "Glass.h"
#include "MicrofacetMaterial.h"
#include "PhongMaterial.h"
#include "Plastic.h"
#include <Textures/Texture.h>
#include <TREngine/Core/Utils/Utils.h>
#include <queue>

using PureTexture_RGB = PureTexture<Color3f>;
using PureTexture_Value = PureTexture<float>;
static std::deque<PureTexture_RGB> textures_rgb;
static std::deque<PureTexture_Value> textures_v;

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
            textures_rgb.push_back(PureTexture_RGB(color));
            return std::make_shared<DiffuseMaterial>(&textures_rgb.back());
        }
    }
    else if (type == "Mirror") {
        auto color = loader::parse_vec3(pNode->GetMember("Color"));
        return std::make_shared<MirrorMaterial>(color);
    }
    else if (type == "Glass") {
        auto color = loader::parse_vec3(pNode->GetMember("Color"));
        auto roughness = loader::parse_vec2(pNode->GetMember("Roughness"));
        auto ior = pNode->GetMember("IOR")->GetFloat();
        return std::make_shared<Glass>(color, ior, roughness);
    }
    else if (type == "Micro") {
        auto color = loader::parse_vec3(pNode->GetMember("Color"));
        auto roughness = loader::parse_vec2(pNode->GetMember("Roughness"));
        auto eta = pNode->GetMember("IOR")->GetFloat();
        auto NDFTypeStr = pNode->GetMember("NDF")->GetString();

        NDFType type;
        if (NDFTypeStr == "Beckmann")
        {
            type = NDFType::Beckmann;
        }
        else if (NDFTypeStr == "GGX")
        {
            type = NDFType::GGX;
        }
        else
        {
            throw std::exception(string_format("Cannot find NDF Type: %s", NDFTypeStr.c_str()).c_str());
        }
        return std::make_shared<MicrofacetMaterial>(color, eta, roughness, type);
    }
    else if (type == "Plastic")
    {
        auto kd = loader::parse_vec3(pNode->GetMember("Kd"));
        auto ks = loader::parse_vec3(pNode->GetMember("Ks"));
        auto roughness = loader::parse_vec2(pNode->GetMember("Roughness"));
        auto NDFTypeStr = pNode->GetMember("NDF")->GetString();

        NDFType type;
        if (NDFTypeStr == "Beckmann")
        {
            type = NDFType::Beckmann;
        }
        else if (NDFTypeStr == "GGX")
        {
            type = NDFType::GGX;
        }
        else
        {
            throw std::exception(string_format("Cannot find NDF Type: %s", NDFTypeStr.c_str()).c_str());
        }
        return std::make_shared<Plastic>(kd, ks, roughness, type);
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
            textures_rgb.push_back(PureTexture_RGB(color));
            return std::make_shared<crystal::PhongMaterial>(&textures_rgb.back(), k);
        }
    }
    else if (type == "OrenNayar")
    {
        if (pNode->HasMember("Kd"))
        {
            throw std::exception("Not implemented");
            /*auto kdTexture = pNode->GetMember("Kd")->GetString();
            auto sigmaTexture = pNode->GetMember("SigmaTex")->GetString();
            return std::make_shared<OrenNayarMaterial>(scene->GetTextureByName(kdTexture), , k);*/
        }
        else
        {
            auto color = loader::parse_vec3(pNode->GetMember("Color"));
            auto sigmaValue = pNode->GetMember("Sigma")->GetFloat();
            textures_rgb.push_back(PureTexture_RGB(color));
            textures_v.push_back(PureTexture_Value(sigmaValue));
            return std::make_shared<OrenNayarMaterial>(&textures_rgb.back(), &textures_v.back());
        }
    }
    else {
        throw std::exception("Invalid material name");
    }
}
