#pragma once
#include "Light.h"
class PointLight : public Light {
public:
    PointLight(glm::vec3 pos, glm::vec3 power);
    ~PointLight() override;

    glm::vec3 Sample_Li(const SurfaceInteraction& hit, const glm::vec2& sample, glm::vec3* endpoint, float* pdf) const override;
    glm::vec3 Flux() const override;
private:
    glm::vec3 _pos, _power;
};
