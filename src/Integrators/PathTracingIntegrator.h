#pragma once
#include <memory>
#include "SamplerIntegrator.h"

namespace crystal
{
    class PathTracingIntegrator : public SamplerIntegrator
    {
    public:
        PathTracingIntegrator(const std::shared_ptr<Sampler>& sampler, int threads, int maxDepth);

        void Preprocess(Scene* scene, Sampler* sampler) override {};
        glm::vec3 Evaluate(const Ray& ray, Scene* scene, Sampler* sampler) override;

    private:
        glm::vec3 eval_rec(const Ray& ray, Scene* scene, Sampler* sampler, int level, bool specular);

        int _maxDepth;

        Spectrum UniformSampleAllLights(const SurfaceInteraction& isec, Scene* scene, Sampler* sampler);
        Spectrum EsimateDirect(const SurfaceInteraction& isec, Scene* scene,
            glm::vec2 sampleLight, glm::vec2 sampleBSDF, const crystal::Light* light, Sampler* sampler);
    };
}