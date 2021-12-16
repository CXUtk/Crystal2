#pragma once
#include <memory>
#include "SamplerIntegrator.h"

class DirectLightingIntegrator : public SamplerIntegrator
{
public:
    DirectLightingIntegrator(const std::shared_ptr<Sampler>& sampler, int threads, int maxDepth);

    glm::vec3 Evaluate(const Ray& ray, Scene* scene, Sampler* sampler) override;

private:
    glm::vec3 eval_rec(const Ray& ray, Scene* scene, Sampler* sampler, int level, bool specular);

    int _maxDepth;
};
