#pragma once
#include <memory>
#include "Integrator.h"
#include <Samplers/Sampler.h>
#include <Core/Geometry.h>

class SamplerIntegrator : public Integrator {
public:
    SamplerIntegrator(const std::shared_ptr<Sampler>& sampler);

    void Preprocess(const std::shared_ptr<Scene>& scene) override;
    void Render(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Camera>& camera, const std::shared_ptr<FrameBuffer>& frameBuffer) override;
    virtual glm::vec3 Evaluate(const Ray& ray, const std::shared_ptr<Scene>& scene, const std::shared_ptr<Sampler>& sampler) = 0;

private:
    std::shared_ptr<Sampler> _sampler;

};