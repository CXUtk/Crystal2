#pragma once
#include "Material.h"
#include <glm/glm.hpp>

class MicrofacetMaterial : public Material {
public:
    MicrofacetMaterial(glm::vec3 color);
    ~MicrofacetMaterial() override;

    std::shared_ptr<BSDF> ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera = true) const override;

private:
    glm::vec3 _color;
};
