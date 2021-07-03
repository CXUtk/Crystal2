#pragma once
#include "Light.h"
class AreaLight : public Light {
public:
    AreaLight() {}
    virtual glm::vec3 EvalEmission(const SurfaceInteraction& isec) const = 0;
};
