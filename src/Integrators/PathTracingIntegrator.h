#pragma once
#include <memory>
#include "SamplerIntegrator.h"

class PathTracingIntegrator : public SamplerIntegrator {
public:
    PathTracingIntegrator(const std::shared_ptr<Sampler>& sampler, int threads);

    glm::vec3 Evaluate(const Ray& ray, Scene* scene, Sampler* sampler) override;

private:
    glm::vec3 eval_rec(const Ray& ray, Scene* scene, Sampler* sampler, int level, bool specular);
};
