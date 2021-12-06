#pragma once
#include "Material.h"
#include <glm/glm.hpp>

class Glass : public Material {
public:
    Glass(glm::vec3 color, float eta);
    ~Glass() override;

    std::shared_ptr<BSDF> ComputeScatteringFunctions(const SurfaceInteraction& isec, 
        bool fromCamera = true) const override;

private:
    glm::vec3 _color;
    float _eta;
};
