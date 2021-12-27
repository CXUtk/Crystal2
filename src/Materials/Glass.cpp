#include <Core/SurfaceInteraction.h>
#include <BSDFs/SpecularReflection.h>
#include <BSDFs/SpecularTransmission.h>
#include <BSDFs/SpecularFresnel.h>
#include "Glass.h"
#include <BSDFs/Models/Fresnel.h>

Glass::Glass(glm::vec3 color, float eta) : _color(color), _eta(eta) {
}

Glass::~Glass() {
}


void Glass::ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera) const
{
    auto N = glm::normalize(isec.GetNormal());
    float etaA = 1.0f, etaB = _eta;
    if (!isec.IsFrontFace()) std::swap(etaA, etaB);
    auto F = std::make_shared<FresnelDielectric>();
    //auto F = std::make_shared<FresnelSchlick>(glm::vec3(0.02));
    isec.GetBSDF()->AddBxDF(std::make_shared<SpecularReflection>(_color, F, etaA, etaB));
    isec.GetBSDF()->AddBxDF(std::make_shared<SpecularTransmission>(_color, F, etaA, etaB));
}