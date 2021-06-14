#pragma once
#include <memory>
#include "SamplerIntegrator.h"

class WhittedIntegrator : public SamplerIntegrator {
public:
    glm::vec3 Evaluate(const Ray& ray, const std::shared_ptr<Scene>& scene) override;
};