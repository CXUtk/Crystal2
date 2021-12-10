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
