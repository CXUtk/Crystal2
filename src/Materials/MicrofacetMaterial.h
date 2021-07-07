#pragma once
#include "Material.h"
#include <glm/glm.hpp>

class MicrofacetMaterial : public Material {
public:
    MicrofacetMaterial(glm::vec3 color, float IOR, float roughness);
    ~MicrofacetMaterial() override;

    std::shared_ptr<BSDF> ComputeScatteringFunctions(const SurfaceInteraction& isec, bool fromCamera = true) const override;

private:
    glm::vec3 _color;
    float _ior, _roughness;
};
