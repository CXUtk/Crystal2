#pragma once
#include "Material.h"
#include <glm/glm.hpp>

class DiffuseMaterial : public Material {
public:
    DiffuseMaterial(glm::vec3 color);
    ~DiffuseMaterial() override;

    std::shared_ptr<BSDF> ComputeScatteringFunctions(SurfaceInteraction& isec, glm::vec3 dir, bool fromCamera = true) const override;

private:
    glm::vec3 _color;
};
