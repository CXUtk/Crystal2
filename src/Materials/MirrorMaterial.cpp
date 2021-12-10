#include "MirrorMaterial.h"
#include <Core/SurfaceInteraction.h>
#include <BSDFs/SpecularReflection.h>

MirrorMaterial::MirrorMaterial(glm::vec3 color) : _R(color) {
}

MirrorMaterial::~MirrorMaterial() {
}

void MirrorMaterial::ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera) const {
    auto N = glm::normalize(isec.GetNormal());
    auto T = glm::normalize(isec.GetDpDu());
    auto B = glm::normalize(glm::cross(T, N));

    isec.GetBSDF()->AddBxDF(std::make_shared<SpecularReflection>(_R, N, 1.f, 1.f, std::make_shared<FresnelNoOp>()));
}
