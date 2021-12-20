#include "MicrofacetMaterial.h"
#include <Core/SurfaceInteraction.h>
#include <BSDFs/MicrofacetReflection.h>
#include <BSDFs/Models/Fresnel.h>
#include <BSDFs/Models/MicrofacetDistribution.h>
#include <BSDFs/Lambertain.h>


MicrofacetMaterial::MicrofacetMaterial(glm::vec3 color, float IOR, glm::vec2 roughness)
    : _color(color), _ior(IOR), _roughness(roughness) {
}

MicrofacetMaterial::~MicrofacetMaterial() {

}

void MicrofacetMaterial::ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera) const {
    float etaA = 1.f, etaB = _ior;
    if (!isec.IsFrontFace()) std::swap(etaA, etaB);
    auto F = std::make_shared<FresnelSchlick>(glm::vec3(.98f));
    auto d = std::make_shared<BeckmannDistribution>(_roughness.x, _roughness.y);

    auto bsdf = std::make_shared<BSDF>(&isec);
    //bsdf->AddBxDF(std::make_shared<Lambertain>(_color, TNB));
    isec.GetBSDF()->AddBxDF(std::make_shared<MicrofacetReflection>(_color, etaA, etaB, F, d));
}
