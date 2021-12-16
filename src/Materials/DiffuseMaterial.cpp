#include "DiffuseMaterial.h"
#include <Core/SurfaceInteraction.h>
#include <BSDFs/Lambertain.h>
#include <algorithm>
#include <cmath>
#include <Textures/Texture.h>


DiffuseMaterial::DiffuseMaterial(const Texture_RGB* Kd) : _Kd(Kd) {
}

DiffuseMaterial::~DiffuseMaterial() {
}

void DiffuseMaterial::ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera) const {
    auto color = _Kd->Evaluate(isec);
    isec.GetBSDF()->AddBxDF(std::make_shared<Lambertain>(color));
}
