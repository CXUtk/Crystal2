#include "DiffuseMaterial.h"
#include <Core/SurfaceInteraction.h>
#include <BSDFs/Lambertain.h>
#include <algorithm>
#include <cmath>

DiffuseMaterial::DiffuseMaterial(glm::vec3 color) : _color(color) {
}

DiffuseMaterial::~DiffuseMaterial() {
}

std::shared_ptr<BSDF> DiffuseMaterial::ComputeScatteringFunctions(const SurfaceInteraction& isec, bool fromCamera) const {
    auto N = glm::normalize(isec.GetNormal());
    auto T = glm::normalize(isec.GetDpDu());
    auto B = glm::normalize(glm::cross(T, N));

    auto uv = isec.GetUV();
    uv *= 5.f;
    bool f1 = glm::mod(uv.x, 1.0f) < 0.5;
    bool f2 = glm::mod(uv.y, 1.0f) < 0.5;

    auto color = _color;
    //if (f1 ^ f2) {
    //    color = glm::vec3(1);
    //}

    auto bsdf = std::make_shared<BSDF>(&isec);
    bsdf->AddBxDF(std::make_shared<Lambertain>(color, glm::mat3(T, N, B)));
    return bsdf;
}
