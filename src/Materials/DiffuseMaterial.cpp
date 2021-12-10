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
    auto N = glm::normalize(isec.GetNormal());
    auto T = glm::normalize(isec.GetDpDu());
    auto B = glm::normalize(glm::cross(T, N));

    auto color = _Kd->Evaluate(isec);

    isec.GetBSDF()->AddBxDF(std::make_shared<Lambertain>(color, glm::mat3(T, N, B)));
}
