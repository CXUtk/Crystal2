#pragma once
#include <Crystal2.h>
#include <memory>
#include <Loaders/ConfigLoader.h>

class RayTracer {
public:
    RayTracer(const config::ConfigInfo& configInfo, const std::shared_ptr<SJsonNode>& configNode, const std::shared_ptr<SJsonNode>& sceneNode);
    std::shared_ptr<FrameBuffer> Trace();

private:
    ~RayTracer();
    std::shared_ptr<Scene> _scene;
    std::shared_ptr<Integrator> _integrator;
    std::shared_ptr<Camera> _camera;

    int _width, _height;
};