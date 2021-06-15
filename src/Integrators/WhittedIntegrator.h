#pragma once
#include <memory>
#include "SamplerIntegrator.h"

class WhittedIntegrator : public SamplerIntegrator {
public:
    WhittedIntegrator(const std::shared_ptr<Sampler>& sampler) : SamplerIntegrator(sampler) {}

    glm::vec3 Evaluate(const Ray& ray, const std::shared_ptr<Scene>& scene) override;
};