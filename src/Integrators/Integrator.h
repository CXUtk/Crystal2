#pragma once
#include <Crystal2.h>
#include <memory>
#include <Core/ConfigInfo.h>
class Integrator {
public:
    static std::shared_ptr<Integrator> LoadIntegrator(const std::shared_ptr<SJsonNode>& configNode, const config::ConfigInfo& configInfo);

    Integrator() {}
    virtual ~Integrator() = 0 {}

    virtual void Preprocess(const std::shared_ptr<Scene>& scene) {}
    virtual void Render(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Camera>& camera, const std::shared_ptr<FrameBuffer>& frameBuffer) = 0;
};