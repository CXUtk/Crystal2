#pragma once

#include <memory>
#include "Scene.h"
#include "FrameBuffer.h"
#include <Loaders/ConfigInfo.h>
#include "Integrators/Integrator.h"
#include <Core/Camera.h>

class RayTracer {
public:
    RayTracer(const config::ConfigInfo& configInfo, const config::SceneInfo& sceneInfo);
    std::shared_ptr<FrameBuffer> Trace();

private:
    ~RayTracer();
    std::shared_ptr<Scene> _scene;
    std::shared_ptr<Integrator> _integrator;
    std::shared_ptr<Camera> _camera;

    int _width, _height;
};