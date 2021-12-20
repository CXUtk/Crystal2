﻿#include <Core/SurfaceInteraction.h>
#include <BSDFs/SpecularReflection.h>
#include <BSDFs/SpecularTransmission.h>
#include <BSDFs/SpecularFresnel.h>
#include "Glass.h"
#include <BSDFs/Models/Fresnel.h>

Glass::Glass(glm::vec3 color, float eta) : _color(color), _eta(eta) {
}

Glass::~Glass() {
}


void Glass::ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera) const {
    auto N = glm::normalize(isec.GetNormal());
    float etaA = 1.0f, etaB = _eta;
    if (!isec.IsFrontFace()) std::swap(etaA, etaB);
    auto F = std::make_shared<FresnelDielectric>();

    isec.GetBSDF()->AddBxDF(std::make_shared<SpecularFresnel>(_color, _color, etaA, etaB, F));
}
