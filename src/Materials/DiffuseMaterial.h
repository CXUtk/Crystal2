#pragma once
#include "Material.h"
#include <glm/glm.hpp>

class DiffuseMaterial : public Material {
public:
    DiffuseMaterial(const Texture_RGB* Kd);
    ~DiffuseMaterial() override;

    void ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera = true) const override;

private:
    const Texture_RGB* _Kd;
};


class OrenNayarMaterial : public Material
{
public:
    OrenNayarMaterial(const Texture_RGB* Kd, const Texture_Float* sigma);
    ~OrenNayarMaterial() override;

    void ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera = true) const override;

private:
    const Texture_RGB* _Kd;
    const Texture_Float* _sigma;
};