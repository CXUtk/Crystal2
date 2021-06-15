#include "Engine.h"
#include "Core/Scene.h"

Engine::Engine(const config::ConfigInfo& configInfo, const config::SceneInfo& sceneInfo) {
    _width = configInfo.Width;
    _height = configInfo.Height;

    _rayTracer = std::make_shared<RayTracer>(configInfo, sceneInfo);
}

Engine::~Engine() {
}

void Engine::Run() {
    auto fb = _rayTracer->Trace();
}
