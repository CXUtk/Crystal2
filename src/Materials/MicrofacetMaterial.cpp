#include "MicrofacetMaterial.h"
#include <Core/SurfaceInteraction.h>
#include <BSDFs/Microfacet.h>
#include <BSDFs/Models/Fresnel.h>
#include <BSDFs/Models/MicrofacetDistribution.h>
#include <BSDFs/Lambertain.h>
#include <BSDFs/FresnelBlend.h>


MicrofacetMaterial::MicrofacetMaterial(const Spectrum& color, float IOR, Vector2f roughness, NDFType ndf)
    : _color(color), _ior(IOR), _roughness(roughness), _ndfType(ndf) {
}

MicrofacetMaterial::~MicrofacetMaterial() {

}

void MicrofacetMaterial::ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera) const
{
    float etaA = 1.f, etaB = _ior;
    if (!isec.IsFrontFace()) std::swap(etaA, etaB);
    //auto F = std::make_shared<FresnelDielectric>();
    auto F = std::make_shared<FresnelSchlick>(glm::vec3(0.99));

    std::shared_ptr<MicrofacetDistribution> distribution = nullptr;
    switch (_ndfType)
    {
    case NDFType::Beckmann:
        distribution = std::make_shared<BeckmannDistribution>(_roughness);
        break;
    case NDFType::GGX:
        distribution = std::make_shared<GGXDistribution>(_roughness);
        break;
    default:
        break;
    }

    auto bsdf = std::make_shared<BSDF>(&isec);
    isec.GetBSDF()->AddBxDF(std::make_shared<MicrofacetReflection>(_color, etaA, etaB, F, distribution));
    //isec.GetBSDF()->AddBxDF(std::make_shared<FresnelBlend>(glm::vec3(0.8, 0.2, 0.3), glm::vec3(0.94), d));
    //isec.GetBSDF()->AddBxDF(std::make_shared<MicrofacetTransmission>(_color, etaA, etaB, F, d));
}