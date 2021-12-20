#pragma once
#include "Material.h"
#include <glm/glm.hpp>

class MicrofacetMaterial : public Material {
public:
    MicrofacetMaterial(glm::vec3 color, float IOR, glm::vec2 roughness);
    ~MicrofacetMaterial() override;

    void ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera = true) const override;

private:
    glm::vec3 _color;
    float _ior;
    glm::vec2 _roughness;
};
