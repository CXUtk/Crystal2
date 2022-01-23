#pragma once
#include <memory>
#include "Integrator.h"
#include <Samplers/Sampler.h>
#include <Core/Geometry.h>
#include <Core/Utils/Threads/FixedThreadPool.h>

namespace crystal
{
    class SamplerIntegrator : public Integrator
    {
    public:
        SamplerIntegrator(const std::shared_ptr<Sampler>& sampler, int threads);

        virtual void Preprocess(Scene* scene, Sampler* sampler) = 0;
        void Render(Scene* scene, Camera* camera,
            FrameBuffer* frameBuffer) override;
        virtual Spectrum Evaluate(const Ray3f& ray, Scene* scene,
            Sampler* sampler) = 0;

    private:
        std::shared_ptr<Sampler> _sampler;
        std::unique_ptr<FixedThreadPool> _threadPool;
        int _numThreads;
    };
}