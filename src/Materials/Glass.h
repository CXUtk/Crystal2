#pragma once
#include "Material.h"
#include <glm/glm.hpp>

class Glass : public Material {
public:
    Glass(const Spectrum& color, float IOR, Vector2f roughness);
    ~Glass() override;

    void ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera = true) const override;

private:
    Spectrum _color;
    float _ior;
    Vector2f _roughness;
};
