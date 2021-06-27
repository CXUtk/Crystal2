#include "DiffuseMaterial.h"
#include <Core/SurfaceInteraction.h>
#include <BSDFs/Lambertain.h>

DiffuseMaterial::DiffuseMaterial(glm::vec3 color) : _color(color) {
}

DiffuseMaterial::~DiffuseMaterial() {
}

std::shared_ptr<BSDF> DiffuseMaterial::ComputeScatteringFunctions(SurfaceInteraction& isec, glm::vec3 dir, bool fromCamera) const {
    auto N = glm::normalize(isec.GetNormal());
    auto T = glm::normalize(isec.GetDpDu());
    auto B = glm::normalize(glm::cross(T, N));

    auto bsdf = std::make_shared<BSDF>(&isec);
    bsdf->AddBxDF(std::make_shared<Lambertain>(_color, glm::mat3(T, N, B)), 1.0f);
    return bsdf;
}
