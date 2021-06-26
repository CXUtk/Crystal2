#pragma once
#include <Crystal2.h>
#include <memory>
#include <Core/ConfigInfo.h>

class Engine {
public:
    Engine(const std::shared_ptr<SJson::SJsonNode>& configInfo, const std::shared_ptr<SJson::SJsonNode>& sceneInfo);
    ~Engine();

    void Run();
private:
    std::shared_ptr<RayTracer> _rayTracer;
    config::ConfigInfo _config;
};