#include "MicrofacetMaterial.h"
#include <Core/SurfaceInteraction.h>
#include <BSDFs/MicrofacetReflection.h>
#include <BSDFs/Models/Fresnel.h>
#include <BSDFs/Models/MicrofacetDistribution.h>
#include <BSDFs/Lambertain.h>


MicrofacetMaterial::MicrofacetMaterial(glm::vec3 color, float IOR, float roughness)
    : _color(color), _ior(IOR), _roughness(roughness) {
}

MicrofacetMaterial::~MicrofacetMaterial() {

}

std::shared_ptr<BSDF> MicrofacetMaterial::ComputeScatteringFunctions(const SurfaceInteraction& isec, bool fromCamera) const {
    auto TNB = glm::mat3(isec.GetDpDu(), isec.GetNormal(), isec.GetDpDv());
    float etaA = 1.f, etaB = _ior;
    if (!isec.IsFrontFace()) std::swap(etaA, etaB);
    auto F = std::make_shared<FresnelSchlick>(glm::vec3(.96f, .96f, .97f));
    auto d = std::make_shared<GGXRTDistribution>(TNB, _roughness);

    auto bsdf = std::make_shared<BSDF>(&isec);
    //bsdf->AddBxDF(std::make_shared<Lambertain>(_color, TNB));
    bsdf->AddBxDF(std::make_shared<MicrofacetReflection>(_color, TNB, etaA, etaB, F, d));
    return bsdf;
}
