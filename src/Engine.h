#pragma once
#include "Loader/SceneInfo.h"
#include "Core/RayTracer.h"
#include <memory>
class Engine {
public:
    Engine(const scene::SceneInfo& sceneInfo);
    ~Engine();

    void Run();

private:
    int _width, _height;
    std::shared_ptr<RayTracer> _rayTracer;
    std::shared_ptr<Scene> _scene;
};