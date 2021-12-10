#pragma once
#include "Material.h"
#include <glm/glm.hpp>

class MirrorMaterial : public Material {
public:
    MirrorMaterial(glm::vec3 color);
    ~MirrorMaterial() override;

    void ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera = true) const override;

private:
    glm::vec3 _R;
};
