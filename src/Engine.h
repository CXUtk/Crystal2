#pragma once

#include <Loaders/ConfigInfo.h>
#include "Core/RayTracer.h"
#include <memory>

class Engine {
public:
    Engine(const config::ConfigInfo& configInfo, const config::SceneInfo& sceneInfo);
    ~Engine();

    void Run();
private:
    int _width, _height;
    std::shared_ptr<RayTracer> _rayTracer;
};