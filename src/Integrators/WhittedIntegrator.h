#pragma once
#include <memory>
#include "Integrator.h"

class WhittedIntegrator : public Integrator {
public:
    static std::shared_ptr<Integrator> LoadIntegrator(const scene::SceneInfo& sceneInfo);

    Integrator() {}
    virtual ~Integrator() = 0 {}

    virtual void Preprocess(const std::shared_ptr<Scene>& scene) {}
    virtual void Render(std::shared_ptr<const Scene> scene, std::shared_ptr<FrameBuffer> frameBuffer) = 0;
};