#pragma once
#include "Material.h"
#include <glm/glm.hpp>

class DiffuseMaterial : public Material {
public:
    DiffuseMaterial(const std::shared_ptr<Texture<glm::vec3>>& Kd);
    ~DiffuseMaterial() override;

    std::shared_ptr<BSDF> ComputeScatteringFunctions(const SurfaceInteraction& isec, bool fromCamera = true) const override;

private:
    std::shared_ptr<Texture<glm::vec3>> _Kd;
};
