#include <Core/SurfaceInteraction.h>
#include <BSDFs/SpecularReflection.h>
#include <BSDFs/SpecularTransmission.h>
#include <BSDFs/SpecularFresnel.h>
#include "Glass.h"
#include <BSDFs/Models/Fresnel.h>
#include <BSDFs/Microfacet.h>

Glass::Glass(const Spectrum& color, float IOR, Vector2f roughness) : _color(color), _ior(IOR), _roughness(roughness) {
}

Glass::~Glass() {
}


void Glass::ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera) const
{
    auto N = glm::normalize(isec.GetNormal());
    float etaA = 1.0f, etaB = _ior;
    if (!isec.IsFrontFace()) std::swap(etaA, etaB);
    auto F = std::make_shared<FresnelDielectric>();
    // auto F = std::make_shared<FresnelSchlick>(glm::vec3(0.02));
    if (_roughness == Vector2f(0.f))
    {
        isec.GetBSDF()->AddBxDF(std::make_shared<SpecularFresnel>(_color, _color, F, etaA, etaB));
    }
    else
    {
        auto d = std::make_shared<GGXDistribution>(_roughness);
        // auto dggx = std::make_shared<GGXDistribution>(_roughness);
        isec.GetBSDF()->AddBxDF(std::make_shared<MicrofacetReflection>(_color, etaA, etaB, F, d));
        isec.GetBSDF()->AddBxDF(std::make_shared<MicrofacetTransmission>(_color, etaA, etaB, F, d));
    }
}