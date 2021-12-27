﻿#include "MicrofacetMaterial.h"
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

void MicrofacetMaterial::ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera) const
{
    float etaA = 1.f, etaB = _ior;
    if (!isec.IsFrontFace()) std::swap(etaA, etaB);
    auto F = std::make_shared<FresnelDielectric>();
    //auto F = std::make_shared<FresnelSchlick>(glm::vec3(0.04));
    auto d = std::make_shared<BeckmannDistribution>(_roughness);

    auto bsdf = std::make_shared<BSDF>(&isec);
    isec.GetBSDF()->AddBxDF(std::make_shared<Lambertain>(glm::vec3(0.8f, 0.2f, 0.3f)));
    isec.GetBSDF()->AddBxDF(std::make_shared<MicrofacetReflection>(_color, etaA, etaB, F, d));
}
