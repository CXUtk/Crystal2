#pragma once
#include "Material.h"
#include <glm/glm.hpp>

class MirrorMaterial : public Material {
public:
    MirrorMaterial(glm::vec3 color);
    ~MirrorMaterial() override;

    std::shared_ptr<BSDF> ComputeScatteringFunctions(const SurfaceInteraction& isec, bool fromCamera = true) const override;

private:
    glm::vec3 _R;
};
