#pragma once
#include "Material.h"
#include <glm/glm.hpp>

class Glass : public Material {
public:
    Glass(glm::vec3 color, float eta);
    ~Glass() override;

    std::shared_ptr<BSDF> ComputeScatteringFunctions(SurfaceInteraction& isec, glm::vec3 dir, bool fromCamera = true) const override;

private:
    glm::vec3 _color;
    float _eta;
};
