#include <Core/SurfaceInteraction.h>
#include <BSDFs/SpecularReflection.h>
#include "MirrorMaterial.h"


MirrorMaterial::MirrorMaterial(glm::vec3 color) : _color(color) {
}

MirrorMaterial::~MirrorMaterial() {
}

std::shared_ptr<BSDF> MirrorMaterial::ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera) const {
    auto N = glm::normalize(isec.GetNormal());

    auto bsdf = std::make_shared<BSDF>(&isec);
    bsdf->AddBxDF(std::make_shared<SpecularReflection>(_color, N), 1.0f);
    return bsdf;
}
