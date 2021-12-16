#include "MirrorMaterial.h"
#include <Core/SurfaceInteraction.h>
#include <BSDFs/SpecularReflection.h>

MirrorMaterial::MirrorMaterial(glm::vec3 color) : _R(color) {
}

MirrorMaterial::~MirrorMaterial() {
}

void MirrorMaterial::ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera) const {
    isec.GetBSDF()->AddBxDF(std::make_shared<SpecularReflection>(_R, 1.f, 1.f, std::make_shared<FresnelNoOp>()));
}
