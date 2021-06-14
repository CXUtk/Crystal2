#pragma once
#include <memory>
#include "Loaders/SceneInfo.h"
class Integrator {
public:
    static std::shared_ptr<Integrator> LoadIntegrator(const scene::SceneInfo& sceneInfo);

    Integrator() {}
    virtual ~Integrator() = 0 {}

    virtual void Preprocess(const std::shared_ptr<Scene>& scene) {}
    virtual void Render(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Camera>& camera, const std::shared_ptr<FrameBuffer>& frameBuffer) = 0;
};