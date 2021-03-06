#pragma once
#include "Material.h"
#include <glm/glm.hpp>

class MicrofacetMaterial : public Material {
public:
    MicrofacetMaterial(const Spectrum& color, float IOR, Vector2f roughness, NDFType ndf);
    ~MicrofacetMaterial() override;

    void ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera = true) const override;

private:
    Spectrum _color;
    float _ior;
    Vector2f _roughness;
    NDFType _ndfType;
};
