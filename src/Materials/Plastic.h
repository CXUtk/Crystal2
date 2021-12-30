#pragma once
#include "Material.h"
#include <glm/glm.hpp>

class Plastic : public Material
{
public:
    Plastic(const Spectrum& kd, const Spectrum& ks, Vector2f roughness, NDFType ndf);
    ~Plastic() override;

    void ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera = true) const override;

private:
    Spectrum _kd, _ks;
    Vector2f _roughness;
    NDFType _ndfType;
};
