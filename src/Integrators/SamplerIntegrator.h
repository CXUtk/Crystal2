#pragma once
#include <memory>
#include "Integrator.h"
#include <Samplers/Sampler.h>
#include <Core/Geometry.h>
#include <Core/Utils/Threads/FixedThreadPool.h>

class SamplerIntegrator : public Integrator {
public:
    SamplerIntegrator(const std::shared_ptr<Sampler>& sampler, int threads);

    void Preprocess(Scene* scene) override;
    void Render(Scene* scene, Camera* camera,
        FrameBuffer* frameBuffer) override;
    virtual glm::vec3 Evaluate(const Ray& ray, Scene* scene,
        Sampler* sampler) = 0;

private:
    std::shared_ptr<Sampler> _samplers[16];
    std::unique_ptr<FixedThreadPool> _threadPool;
    int _numThreads;
};