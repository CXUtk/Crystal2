#include "MicrofacetMaterial.h"
#include <Core/SurfaceInteraction.h>
#include <BSDFs/MicrofacetReflection.h>
#include <BSDFs/Models/Fresnel.h>
#include <BSDFs/Models/MicrofacetDistribution.h>


MicrofacetMaterial::MicrofacetMaterial(glm::vec3 color, float IOR, float roughness)
    : _color(color), _ior(IOR), _roughness(roughness) {
}

MicrofacetMaterial::~MicrofacetMaterial() {

}

std::shared_ptr<BSDF> MicrofacetMaterial::ComputeScatteringFunctions(const SurfaceInteraction& isec, bool fromCamera) const {
    auto TNB = glm::mat3(isec.GetDpDu(), isec.GetNormal(), isec.GetDpDv());
    float etaA = _ior, etaB = 1.0f;
    if (!isec.IsFrontFace()) std::swap(etaA, etaB);
    auto F = std::make_shared<FresnelDielectric>(etaA, etaB);
    auto d = std::make_shared<GGXDistribution>(TNB, _roughness, _roughness );

    auto bsdf = std::make_shared<BSDF>(&isec);
    bsdf->AddBxDF(std::make_shared<MicrofacetReflection>(_color, TNB, F, d));
    return bsdf;
}
